

#include "scheduler.hpp"

#include "criticalsection.hpp"
#include "infra/List.hpp"
#include "infra/SortedList.hpp"
#include "stm32f103xb.h"
#include "stm32f1xx_ll_gpio.h"
#include "utils.hpp"

#include <array>
#include <cassert>
#include <core_cm3.h>
#include <cstdint>

uint32_t systicks = 0;

extern "C"
{
    Task* volatile currentTaskControlBlock = nullptr;
}

auto delayedTaskCompare = [](auto base, auto other) { return other->tickDelay < base->tickDelay; };

static List<Task> blockedTasks;
static SortedList<Task, decltype(delayedTaskCompare)> delayedTasks{delayedTaskCompare};
static List<Task> readyTasks;

static void task1Handler();
static void task2Handler();

static void taskIdle()
{
    while (1)
    {
        __WFE();
    }
}

static Task::WithStack<128> task1{task1Handler, {GPIOC, LL_GPIO_PIN_14}};
static Task::WithStack<128> task2{task2Handler, {GPIOC, LL_GPIO_PIN_15}};

static Task::WithStack<32> idleTask{taskIdle, {GPIOA, LL_GPIO_PIN_0}};

static void task1Handler()
{
    while (1)
    {
        LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);

        DelayTask(task2);
        DelayTask(9999);
    }
}

static void task2Handler()
{
    while (1)
    {
        LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);

		DelayTask(task1);
        DelayTask(9999);
    }
}

void scheduler_yield()
{
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    asm volatile("isb");
    asm volatile("dsb" ::: "memory");
}

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

            readyTasks.AddBack(*item);

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
    readyTasks.AddBack(task1.queueItem);
    readyTasks.AddBack(task2.queueItem);

    currentTaskControlBlock = &idleTask;

    asm volatile("cpsie i        \n"
                 "cpsie f        \n"
                 "dsb            \n"
                 "isb            \n"
                 "svc 0          \n"
                 "nop            \n"
                 "bx lr");
}

void TaskScheduler()
{
    if (currentTaskControlBlock->StackSafe() == false)
    {
        asm volatile("bkpt");
    }

    if (currentTaskControlBlock->gpioDebug.port != nullptr)
    {
        LL_GPIO_ResetOutputPin((GPIO_TypeDef*)currentTaskControlBlock->gpioDebug.port,
                               currentTaskControlBlock->gpioDebug.pin);
    }

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

    if (currentTaskControlBlock->StackSafe() == false)
    {
        asm volatile("bkpt");
    }

    if (currentTaskControlBlock->gpioDebug.port != nullptr)
    {
        LL_GPIO_SetOutputPin((GPIO_TypeDef*)currentTaskControlBlock->gpioDebug.port,
                             currentTaskControlBlock->gpioDebug.pin);
    }
}

void DelayTask(uint32_t ticks)
{
    ScopedCriticalSection critical;
    
    DelayTask(*currentTaskControlBlock, ticks);

    scheduler_yield();
}

void DelayTask(Task& task, uint32_t ticks)
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