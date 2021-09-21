#include "scheduler.hpp"
#include "SEGGER_RTT.h"
#include "kernel/criticalsection.hpp"
#include "kernel/getkernel.hpp"
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

void YieldTask()
{
    kernel::GetScheduler().Yield();
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

    DelayTask(kernel::GetScheduler().CurrentTaskControlBlock(), ticks);

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
                auto& task = *iter;
                ++iter;

                if (task.Owner().tickDelay <= static_cast<std::uint32_t>(kernelInstance->systicks))
                {
                    TaskList<>::erase(task);

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
            }

            return mustSwitch;
        }

        return !kernelInstance->readyTasksV2.empty();
    }

    void* Scheduler::SwitchContext(void* stackPointer)
    {
        CurrentTaskControlBlock().GetStack().SetStackPointer(stackPointer);

        if (CurrentTaskControlBlock().GetStack().IsSafe() == false)
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

        if (CurrentTaskControlBlock().GetStack().IsSafe() == false)
        {
            kernel::port::breakpoint();
        }

        return CurrentTaskControlBlock().GetStack().GetStackPointer();
    }

    StatusCode Scheduler::Add(TaskControlBlock& ctrlBlock)
    {
        ctrlBlock.GetStack().Initialize(&ctrlBlock.Owner());

        readyTasksV2.push(ctrlBlock);

        return StatusCode::Ok;
    }

    StatusCode Scheduler::Yield()
    {
        ScopedCriticalSection critical;

        readyTasksV2.push(CurrentTaskControlBlock());

        TriggerTaskSwitch();

        return StatusCode::Ok;
    }

    StatusCode Scheduler::Yield(TaskControlBlock& ctrlBlock)
    {
        ScopedCriticalSection critical;

        readyTasksV2.push(ctrlBlock);

        TriggerTaskSwitch();

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

        return unblockReason == UnblockReason::Request ? StatusCode::Ok
            : unblockReason == UnblockReason::Timeout  ? StatusCode::Timeout
                                                       : StatusCode::Interrupted;
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
    }

    Scheduler::Scheduler(MainThread& mainThread)
        : idleTask{"idle", taskIdle} // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
    {
        currentTaskControlBlock = &mainThread.GetTaskControlBlock();

        kernelInstance = this;

        idleTask.GetTaskControlBlock().Priority(UINT32_MAX - 1);
        idleTask.Start();
    }

    Scheduler::~Scheduler()
    {
        kernelInstance = nullptr;
    }

    auto Scheduler::CurrentTask() -> RunnableTask&
    {
        return GetScheduler().CurrentTaskControlBlock().Owner();
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