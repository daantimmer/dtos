

#include "scheduler.hpp"

#include "infra/DoubleLinkedList.hpp"
#include "stm32f103xb.h"
#include "stm32f1xx_ll_gpio.h"
#include "utils.hpp"

#include <array>
#include <cassert>
#include <core_cm3.h>
#include <cstdint>

uint32_t criticalNestingCounter = 0;

void EnterCriticalSection()
{
    DisableInterrupts();

    criticalNestingCounter++;
}
void ExitCriticalSection()
{
    if (criticalNestingCounter > 0)
    {
        criticalNestingCounter--;

        if (criticalNestingCounter == 0)
        {
            EnableInterrupts();
        }
    }
    else
    {
        assert(false);
    }
}

extern "C"
{
    Task* volatile currentTaskControlBlock = nullptr;
}

static std::array<uint32_t, 32 + 2> stackTask1;
static std::array<uint32_t, 32 + 2> stackTask2;
static std::array<uint32_t, 32 + 2> stackTask3;

static void task1Handler() __attribute__((naked));
static void task2Handler() __attribute__((naked));

static DoubleLinkedList<Task> blockedTasks;
static DoubleLinkedList<Task> delayedTasks;
static DoubleLinkedList<Task> readyTasks;

static void task1Handler()
{
    while (1)
    {
        LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
        __WFE();
    }
}

static void task2Handler()
{
    while (1)
    {
        LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
        __WFE();
    }
}

static void taskIdle()
{
    while (1)
    {
        __WFE();
    }
}

static Task task1{task1Handler, stackTask1.data(), stackTask1.size()};
static Task task2{task2Handler, stackTask2.data(), stackTask2.size()};

static Task idleTask{taskIdle, stackTask3.data(), stackTask3.size()};

static DoubleLinkedList<Task>::Item task1Item{task1};
static DoubleLinkedList<Task>::Item task2Item{task2};
static DoubleLinkedList<Task>::Item idleTaskItem{idleTask};

void scheduler_yield()
{
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    asm volatile("dsb" ::: "memory");
    asm volatile("isb");
}

void startFirstTask()
{
    readyTasks.AddBack(task1Item);
    readyTasks.AddBack(task2Item);
    readyTasks.AddBack(idleTaskItem);

    currentTaskControlBlock = &task1;

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
    if (readyTasks.Empty() == false)
    {
        auto nextTask = readyTasks.PeekFront();

        if (nextTask != nullptr)
        {
            readyTasks.PopFront();
            readyTasks.AddBack(*nextTask);

            currentTaskControlBlock = &(nextTask->Object());
        }
    }
}
