

#include "scheduler.hpp"

// #include "infra/List.hpp"
// #include "infra/SortedList.hpp"
#include "elib/intrusivelist.hpp"
#include "kernel/criticalsection.hpp"
#include "kernel/kernel.hpp"
#include "kernel/port/systemtick.hpp"
#include "kernel/spinlock.hpp"
#include "kernel/task.hpp"
#include "stm32f103xb.h"
#include "stm32f1xx_ll_gpio.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstdint>

extern "C"
{
    RunnableTask* volatile currentTaskControlBlock = nullptr;
}

void TriggerTaskSwitch()
{
    // Ignore 'do not use c-style cast' warning. Can't be avoided here
    // NOLINTNEXTLINE
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    asm volatile("isb");
    asm volatile("dsb" ::: "memory");
}

auto SchedulerTick() -> bool
{
    kernel::GetKernel().systicks++;

    if (!kernel::GetKernel().delayedTasks.empty())
    {
        bool mustSwitch = false;

        auto iter = kernel::GetKernel().delayedTasks.begin();
        const auto end = kernel::GetKernel().delayedTasks.end();

        for (; iter != end;)
        {
            if (auto& task = *iter; task.tickDelay <= static_cast<std::uint32_t>(kernel::GetKernel().systicks))
            {
                iter = kernel::RunnableTaskList::erase(iter);

                kernel::GetKernel().readyTasks.insert(task);

                if (task.interval != 0)
                {
                    task.tickDelay = static_cast<std::uint32_t>(kernel::GetKernel().systicks) + task.interval;
                }
                else
                {
                    task.tickDelay = 0;
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

    return !kernel::GetKernel().readyTasks.empty();
}

void TaskScheduler()
{
    // if (currentTaskControlBlock->StackSafe() == false)
    // {
    //     asm volatile("bkpt");
    // }

    // if (currentTaskControlBlock->gpioDebug.port != nullptr)
    // {
    //     LL_GPIO_ResetOutputPin((GPIO_TypeDef*)currentTaskControlBlock->gpioDebug.port,
    //                            currentTaskControlBlock->gpioDebug.pin);
    // }

    if (!kernel::GetKernel().readyTasks.empty())
    {
        // kernel::GetKernel().readyTasks.push_back(*currentTaskControlBlock);

        currentTaskControlBlock = &*kernel::GetKernel().readyTasks.begin();
    }
    else
    {
        currentTaskControlBlock = &kernel::GetKernel().GetIdleTask();
    }

    // if (currentTaskControlBlock->StackSafe() == false)
    // {
    //     asm volatile("bkpt");
    // }

    // if (currentTaskControlBlock->gpioDebug.port != nullptr)
    // {
    //     LL_GPIO_SetOutputPin((GPIO_TypeDef*)currentTaskControlBlock->gpioDebug.port,
    //                          currentTaskControlBlock->gpioDebug.pin);
    // }
}

void YieldTask()
{
    ScopedCriticalSection critical;

    kernel::GetKernel().readyTasks.insert(*currentTaskControlBlock);

    TriggerTaskSwitch();
}

void BlockTask()
{
    ScopedCriticalSection critical;

    kernel::GetKernel().blockedTasks.insert(*currentTaskControlBlock);

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

void DelayTask(RunnableTask& task, std::uint32_t ticks)
{
    ScopedCriticalSection critical;

    task.tickDelay = static_cast<std::uint32_t>(kernel::GetKernel().systicks) + ticks;

    kernel::GetKernel().delayedTasks.insert(task);
}