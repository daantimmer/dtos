
#include "SEGGER_RTT.h"
#include "hal/datamodel.hpp"
#include "hal/port/peripherals.hpp"
#include "kernel/container/heap.hpp"
#include "kernel/getkernel.hpp"
#include "kernel/lockable.hpp"
#include "kernel/mainthread.hpp"
#include "kernel/port/systemclock.hpp"
#include "kernel/port/systemtick.hpp"
#include "kernel/scheduler.hpp"
#include "kernel/synchronisation/fifo.hpp"
#include "kernel/synchronisation/mutex.hpp"
#include "kernel/synchronisation/semaphore.hpp"
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include <cstdint>

extern "C"
{
    extern TIM_HandleTypeDef htim3;
    extern ADC_HandleTypeDef hadc1;
}

namespace
{
    volatile std::uint32_t count = 0;
    volatile std::uint32_t adcValue = 0;
    auto temperatureValueSemaphore = kernel::Semaphore{};

    auto task1Handler = [](const kernel::RunnableTask&, void*) // NOLINT
    {
        while (true)
        {
            SEGGER_RTT_TerminalOut(0, "Task1\n");
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
            DelayTask(std::chrono::milliseconds{1000});
        }
    };

    auto task2Handler = [](const kernel::RunnableTask&, void* semaphoreVoidPtr) // NOLINT
    {
        auto& semaphore = *static_cast<kernel::Semaphore*>(semaphoreVoidPtr);

        HAL_TIM_Base_Start_IT(&htim3);
        HAL_ADC_Start_IT(&hadc1);

        while (true)
        {
            semaphore.Acquire();
        }
    };

    kernel::MainThread mainThread{};

    constexpr auto defaultstacksize = 256U;
    kernel::StaticTask<defaultstacksize> task1{"task1",
                                               +task1Handler,
                                               nullptr}; // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)

    kernel::StaticTask<defaultstacksize> task2{
        "task2",
        +task2Handler,
        &temperatureValueSemaphore}; // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    const auto value = HAL_ADC_GetValue(hadc);
    adcValue = (value + adcValue) / 2;
    temperatureValueSemaphore.Release();
}

extern "C" void RealMain()
{
    SEGGER_RTT_Init();

    kernel::Scheduler kernel{mainThread};

    task1.priority = 1;
    task2.priority = 0;

    kernel::GetScheduler().readyTasksV2.push(task1.GetTaskControlBlock());
    kernel::GetScheduler().readyTasksV2.push(task2.GetTaskControlBlock());

    // hal::Notification::Callback l1cb{[] { SEGGER_RTT_printf(0, "INIT L1\r\n"); }};
    // hal::Notification::Callback l2cb{[] { SEGGER_RTT_printf(0, "INIT L2\r\n"); }};

    // hal::Value<hal::Foo>::Attach(l1cb);
    // hal::Value<hal::Foo>::Attach(l2cb);

    __enable_irq();

    while (true)
    {
    }
}