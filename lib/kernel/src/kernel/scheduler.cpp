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
// #include "stm32f103xb.h"
// #include "stm32f1xx.h"
// #include "stm32f1xx_ll_gpio.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstdint>

extern "C"
{
    kernel::RunnableTask* volatile currentTaskControlBlock = nullptr;
}

namespace
{
    kernel::Scheduler* kernelInstance = nullptr;

    void taskIdle(const kernel::Task&, void*)
    {
        while (true)
        {
            kernel::port::WaitForEvent();
        }
    }
}

auto SchedulerTick() -> bool
{
    return kernelInstance->Tick();
}

void TaskScheduler()
{
    if (currentTaskControlBlock->StackSafe() == false)
    {
        kernel::port::breakpoint();
    }

    if (!kernelInstance->readyTasksV2.empty())
    {
        currentTaskControlBlock = kernelInstance->readyTasksV2.top().task;
    }
    else
    {
        currentTaskControlBlock = &kernelInstance->GetIdleTask();
    }

    // SEGGER_RTT_printf(0,
    //                   "Switch to %s stack >%u<\r\n",
    //                   currentTaskControlBlock->name,
    //                   currentTaskControlBlock->StackAvailable());

    if (currentTaskControlBlock->StackSafe() == false)
    {
        kernel::port::breakpoint();
    }
}

void YieldTask()
{
    ScopedCriticalSection critical;

    kernelInstance->readyTasksV2.push(currentTaskControlBlock->queueItemV2);

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

void DelayTask(kernel::RunnableTask& task, std::uint32_t ticks)
{
    ScopedCriticalSection critical;

    task.tickDelay = static_cast<std::uint32_t>(kernelInstance->systicks) + ticks;

    kernelInstance->delayedTasksV2.push(task.queueItemV2);
}

auto kernel::Scheduler::Tick() -> bool
{
    kernelInstance->systicks++;

    if (!kernelInstance->delayedTasksV2.empty())
    {
        bool mustSwitch = false;

        auto iter = kernelInstance->delayedTasksV2.begin();
        const auto end = kernelInstance->delayedTasksV2.end();

        for (; iter != end;)
        {
            if (auto& task = *iter; task.task->tickDelay <= static_cast<std::uint32_t>(kernelInstance->systicks))
            {
                iter = kernelInstance->delayedTasksV2.erase(iter);

                kernelInstance->readyTasksV2.push(task.task->queueItemV2);

                if (task.task->interval != 0)
                {
                    task.task->tickDelay = static_cast<std::uint32_t>(kernelInstance->systicks) + task.task->interval;
                }
                else
                {
                    task.task->tickDelay = 0;
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

kernel::StatusCode kernel::Scheduler::Block(TaskList<>& blockList, const kernel::UnblockFunction& externalUnblockHook)
{
    return Block(blockList, *currentTaskControlBlock, externalUnblockHook);
}

kernel::StatusCode kernel::Scheduler::Block(TaskList<>& blockList,
                                            RunnableTask& task,
                                            const kernel::UnblockFunction& externalUnblockHook)
{
    auto unblockReason = kernel::UnblockReason::Undefined;
    auto isCurrentThread = &task == currentTaskControlBlock;
    auto unblockReasonFunctor = [&unblockReason, &externalUnblockHook](RunnableTask& task, UnblockReason reason) {
        unblockReason = reason;
        if (static_cast<bool>(externalUnblockHook))
        {
            externalUnblockHook(task, reason);
        }
    };

    {
        const kernel::InterruptMasking interruptMasking;

        if (const auto ret = InternalBlock(blockList, task, unblockReasonFunctor); ret != kernel::StatusCode::Ok)
        {
            return ret;
        }

        if (isCurrentThread == false)
        {
            return kernel::StatusCode::Ok;
        }
    }

    ForceContextSwitch();

    return unblockReason == kernel::UnblockReason::Request
        ? kernel::StatusCode::Ok
        : unblockReason == kernel::UnblockReason::Timeout ? kernel::StatusCode::Timeout
                                                          : kernel::StatusCode::Interrupted;
}

void kernel::Scheduler::Unblock(RunnableTask& task)
{
    const kernel::InterruptMasking interruptMasking;

    InternalUnblock(task, UnblockReason::Request);

    TriggerTaskSwitch(); // ?? maybe
}

kernel::StatusCode
    kernel::Scheduler::InternalBlock(TaskList<>& blockList, RunnableTask& task, UnblockFunction unblockFunction)
{
    blockList.transfer(task.queueItemV2);

    task.BlockHook(unblockFunction);

    // SEGGER_RTT_printf(0, "Block %s @ %p\r\n", task.name, &task);

    return StatusCode::Ok;
}

void kernel::Scheduler::InternalUnblock(RunnableTask& task, UnblockReason unblockReason)
{
    readyTasksV2.transfer(task.queueItemV2);

    task.UnblockHook(unblockReason);

    // SEGGER_RTT_printf(0, "Unblock %s @ %p\r\n", task.name, &task);
}

kernel::Scheduler::Scheduler(MainThread& mainThread)
    : idleTask{"idle", taskIdle /*, {GPIOA, LL_GPIO_PIN_0}*/} // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
{
    currentTaskControlBlock = &mainThread;
    // idleTask.name = "Idle";

    kernelInstance = this;

    idleTask.priority = UINT32_MAX - 1;

    readyTasksV2.push(idleTask.queueItemV2);
    // readyTasks.insert(idleTask);
}

kernel::Scheduler::~Scheduler()
{
    kernelInstance = nullptr;
}

auto kernel::Scheduler::CurrentTask() -> RunnableTask&
{
    return *currentTaskControlBlock;
}

auto kernel::Scheduler::GetIdleTask() const -> RunnableTask&
{
    return idleTask;
}

namespace kernel
{
    auto GetKernel() -> Scheduler&
    {
        return *kernelInstance;
    }

    auto GetScheduler() -> Scheduler&
    {
        return *kernelInstance;
    }
}