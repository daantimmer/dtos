

#include "scheduler.hpp"

#include "infra/List.hpp"
#include "infra/SortedList.hpp"
#include "kernel/criticalsection.hpp"
#include "kernel/mutex.hpp"
#include "kernel/port/systemtick.hpp"
#include "stm32f103xb.h"
#include "stm32f1xx_ll_gpio.h"

#include <array>
#include <cassert>
#include <chrono>
#include <cstdint>

std::uint32_t systicks = 0;

extern "C"
{
    RunnableTask* volatile currentTaskControlBlock = nullptr;
}

void TriggerTaskSwitch()
{
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    asm volatile("isb");
    asm volatile("dsb" ::: "memory");
}

auto delayedTaskCompare = [](auto base, auto other) { return other->tickDelay < base->tickDelay; };
auto readyTasksCompare = [](auto base, auto other) { return other->priority < base->priority; };

static List<RunnableTask> blockedTasks;
static SortedList<RunnableTask, decltype(delayedTaskCompare)> delayedTasks{delayedTaskCompare};
static SortedList<RunnableTask, decltype(readyTasksCompare)> readyTasks{readyTasksCompare};

static void taskIdle()
{
    while (1)
    {
        __WFE();
    }
}

static auto task1Handler = []() {
    while (1)
    {
        LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);

        DelayTask(std::chrono::milliseconds{250});
    }
};

static auto task2Handler = []() {
    DelayTask(std::chrono::milliseconds{125});

    while (1)
    {
        LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);

        DelayTask(std::chrono::milliseconds{250});
    }
};

static Task::WithStack<128> task1{task1Handler, {GPIOC, LL_GPIO_PIN_14}};
static Task::WithStack<128> task2{task2Handler, {GPIOC, LL_GPIO_PIN_15}};

static Task::WithStack<32> idleTask{taskIdle, {GPIOA, LL_GPIO_PIN_0}};

bool SchedulerTick()
{
    systicks++;

    if (delayedTasks.Empty() == false)
    {
        bool mustSwitch = false;

        auto item = delayedTasks.PeekFront();

        while ((item != nullptr) && (*item)->tickDelay < systicks)
        {
            delayedTasks.PopFront();

            readyTasks.Insert(*item);

            item = delayedTasks.PeekFront();

            mustSwitch = true;
        }

        return mustSwitch;
    }
    else
    {
        return readyTasks.Empty() == false;
    }
}

void startFirstTask()
{
    readyTasks.Insert(task1.queueItem);
    readyTasks.Insert(task2.queueItem);
    readyTasks.Insert(idleTask.queueItem);
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

    if (readyTasks.Empty() == false)
    {
        auto nextTask = readyTasks.PeekFront();

        readyTasks.PopFront();

        currentTaskControlBlock = *nextTask;
    }
    else
    {
        currentTaskControlBlock = &idleTask;
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

    readyTasks.Insert(currentTaskControlBlock->queueItem);

    TriggerTaskSwitch();
}

void BlockTask()
{
    ScopedCriticalSection critical;

    if (readyTasks.Contains(currentTaskControlBlock->queueItem) == true)
    {
        readyTasks.Remove(currentTaskControlBlock->queueItem);
    }

    blockedTasks.AddBack(currentTaskControlBlock->queueItem);

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

void DelayTask(uint32_t ticks)
{
    ScopedCriticalSection critical;

    DelayTask(*currentTaskControlBlock, ticks);

    TriggerTaskSwitch();
}

void DelayTask(RunnableTask& task, uint32_t ticks)
{
    ScopedCriticalSection critical;

    if (blockedTasks.Contains(task.queueItem) == true)
    {
        return;
    }

    if (readyTasks.Contains(task.queueItem) == true)
    {
        readyTasks.Remove(task.queueItem);
    }

    task.tickDelay = systicks + ticks;

    delayedTasks.Insert(task.queueItem);
}