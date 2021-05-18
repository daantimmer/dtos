#include "scheduler.hpp"
#include "SEGGER_RTT.h"
// #include "elib/intrusivelist.hpp"
#include "kernel/criticalsection.hpp"
#include "kernel/interruptMasking.hpp"
#include "kernel/mainthread.hpp"
#include "kernel/port/breakpoint.hpp"
#include "kernel/port/systemtick.hpp"
#include "kernel/port/triggerTaskSwitch.hpp"
#include "kernel/port/waitForEvent.hpp"
#include "kernel/scheduler.hpp"
#include "kernel/spinlock.hpp"
#include "kernel/task.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstdint>

extern "C"
{
    kernel::TaskControlBlock* volatile currentTaskControlBlock = nullptr;
}

namespace
{
    kernel::Scheduler* kernelInstance = nullptr;

    void taskIdle(const kernel::RunnableTask& /*unused*/, void* /*unused*/)
    {
        while (true)
        {
            kernel::port::WaitForEvent();
        }
    }

    kernel::StatusCode InternalBlock(kernel::TaskList<>& blockList,
                                     kernel::TaskControlBlock& ctrlBlock,
                                     const kernel::UnblockFunction& unblockFunction)
    {
        blockList.transfer(ctrlBlock);

        ctrlBlock.BlockHook(unblockFunction);

        return kernel::StatusCode::Ok;
    }
}

auto SchedulerTick() -> bool
{
    return kernelInstance->Tick();
}

void TaskScheduler()
{
    if (currentTaskControlBlock->GetStack().IsSafe() == false)
    {
        kernel::port::breakpoint();
    }

    if (!kernelInstance->readyTasksV2.empty())
    {
        currentTaskControlBlock = &kernelInstance->readyTasksV2.top();
    }
    else
    {
        currentTaskControlBlock = &kernelInstance->GetIdleTask();
    }

    // SEGGER_RTT_printf(0,
    //                   "Switch to %s stack >%u<\r\n",
    //                   currentTaskControlBlock->name,
    //                   currentTaskControlBlock->StackAvailable());

    if (currentTaskControlBlock->GetStack().IsSafe() == false)
    {
        kernel::port::breakpoint();
    }
}

void YieldTask()
{
    ScopedCriticalSection critical;

    kernelInstance->readyTasksV2.push(*currentTaskControlBlock);

    TriggerTaskSwitch();
}

void DelayTask(std::chrono::microseconds delay)
{
    DelayTask(std::chrono::duration_cast<systemtick::ticks>(delay).count());
}

void DelayTask(std::chrono::milliseconds delay)
{
    DelayTask(std::chrono::duration_cast<std::chrono::microseconds>(delay));
}

void DelayTask(std::uint32_t ticks)
{
    ScopedCriticalSection critical;

    DelayTask(*currentTaskControlBlock, ticks);

    TriggerTaskSwitch();
}

void DelayTask(kernel::TaskControlBlock& ctrlBlock, std::uint32_t ticks)
{
    ScopedCriticalSection critical;

    ctrlBlock.Owner().tickDelay = static_cast<std::uint32_t>(kernelInstance->systicks) + ticks;

    kernelInstance->delayedTasksV2.push(ctrlBlock);
}

namespace kernel
{
    auto Scheduler::Tick() -> bool // NOLINT
    {
        kernelInstance->systicks++;

        if (!kernelInstance->delayedTasksV2.empty())
        {
            bool mustSwitch = false;

            auto iter = kernelInstance->delayedTasksV2.begin();
            const auto end = kernelInstance->delayedTasksV2.end();

            for (; iter != end;)
            {
                if (auto& task = *iter; task.Owner().tickDelay <= static_cast<std::uint32_t>(kernelInstance->systicks))
                {
                    iter = TaskList<>::erase(iter);

                    kernelInstance->readyTasksV2.push(task);

                    if (task.Owner().interval != 0)
                    {
                        task.Owner().tickDelay
                            = static_cast<std::uint32_t>(kernelInstance->systicks) + task.Owner().interval;
                    }
                    else
                    {
                        task.Owner().tickDelay = 0;
                    }

                    mustSwitch = true;
                }
                else
                {
                    ++iter;
                }
            }

            return mustSwitch;
        }

        return !kernelInstance->readyTasksV2.empty();
    }

    StatusCode Scheduler::Add(TaskControlBlock& ctrlBlock)
    {
        ctrlBlock.GetStack().Initialize(&ctrlBlock.Owner());
        readyTasksV2.push(ctrlBlock);

        return StatusCode::Ok;
    }

    StatusCode Scheduler::Block(TaskList<>& blockList, const UnblockFunction& externalUnblockHook)
    {
        return Block(blockList, *currentTaskControlBlock, externalUnblockHook);
    }

    StatusCode Scheduler::Block(TaskList<>& blockList, // NOLINT(readability-convert-member-functions-to-static)
                                TaskControlBlock& ctrlBlock,
                                const UnblockFunction& externalUnblockHook)
    {
        auto unblockReason = UnblockReason::Undefined;

        auto isCurrentThread = &ctrlBlock == currentTaskControlBlock;

        auto unblockReasonFunctor
            = [&unblockReason, &externalUnblockHook](TaskControlBlock& ctrlBlock, UnblockReason reason) {
                  unblockReason = reason;
                  if (static_cast<bool>(externalUnblockHook))
                  {
                      externalUnblockHook(ctrlBlock, reason);
                  }
              };

        {
            const InterruptMasking interruptMasking;

            if (const auto ret = InternalBlock(blockList, ctrlBlock, unblockReasonFunctor); ret != StatusCode::Ok)
            {
                return ret;
            }

            if (isCurrentThread == false)
            {
                return StatusCode::Ok;
            }
        }

        ForceContextSwitch();

        return unblockReason == UnblockReason::Request
            ? StatusCode::Ok
            : unblockReason == UnblockReason::Timeout ? StatusCode::Timeout : StatusCode::Interrupted;
    }

    void Scheduler::Unblock(TaskControlBlock& ctrlBlock)
    {
        const InterruptMasking interruptMasking;

        InternalUnblock(ctrlBlock, UnblockReason::Request);

        TriggerTaskSwitch(); // ?? maybe
    }

    void Scheduler::InternalUnblock(TaskControlBlock& ctrlBlock, UnblockReason unblockReason)
    {
        readyTasksV2.transfer(ctrlBlock);

        ctrlBlock.UnblockHook(unblockReason);

        // SEGGER_RTT_printf(0, "Unblock %s @ %p\r\n", task.name, &task);
    }

    Scheduler::Scheduler(MainThread& mainThread)
        : idleTask{"idle", taskIdle /*, {GPIOA, LL_GPIO_PIN_0}*/} // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
    {
        currentTaskControlBlock = &mainThread.GetTaskControlBlock();
        // idleTask.name = "Idle";

        kernelInstance = this;

        idleTask.GetTaskControlBlock().Priority(UINT32_MAX - 1);

        readyTasksV2.push(idleTask.GetTaskControlBlock());
        // readyTasks.insert(idleTask);
    }

    Scheduler::~Scheduler()
    {
        kernelInstance = nullptr;
    }

    auto Scheduler::CurrentTask() -> RunnableTask&
    {
        return currentTaskControlBlock->Owner();
    }

    TaskControlBlock& Scheduler::GetIdleTask() const
    {
        return idleTask.GetTaskControlBlock();
    }

    auto GetKernel() -> Scheduler&
    {
        return *kernelInstance;
    }

    auto GetScheduler() -> Scheduler&
    {
        return *kernelInstance;
    }
}