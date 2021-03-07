
// Rtos.cpp : Defines the entry point for the application.
//

#include "SEGGER_RTT.h"
#include "hal/port/peripherals.hpp"
#include "infra/List.hpp"
#include "kernel/getkernel.hpp"
#include "kernel/lockable.hpp"
#include "kernel/mainthread.hpp"
#include "kernel/port/systemclock.hpp"
#include "kernel/port/systemtick.hpp"
#include "kernel/scheduler.hpp"
#include "kernel/synchronisation/mutex.hpp"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_utils.h"
#include <cstdint>

namespace
{
    void SetupClocking()
    {
        SystemClock_Setup();
        systemtick::Setup();
    }

    auto task1Handler = [](const kernel::Task&, void* param) // NOLINT
    {
        int cntr = 10;
        auto& mutex = *static_cast<kernel::Mutex*>(param);
        mutex.Lock();
        while (true)
        {
            LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
            DelayTask(std::chrono::milliseconds{250});

            if (cntr > 0)
            {
                --cntr;
                if (cntr == 0)
                {
                    mutex.Unlock();
                }
            }
        }
    };

    auto task2Handler = [](const kernel::Task&, void* param) { // NOLINT
        auto& mutex = *static_cast<kernel::Mutex*>(param);

        DelayTask(std::chrono::milliseconds{1});

        mutex.Lock();

        DelayTask(std::chrono::milliseconds{125});

        while (true)
        {
            LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
            DelayTask(std::chrono::milliseconds{250});
        }
    };

    kernel::MainThread mainThread{};
    kernel::Mutex mutex{};

    constexpr auto defaultstacksize = 256U;
    kernel::Task::WithStack<defaultstacksize> task1{task1Handler,
                                                    &mutex}; // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)

    kernel::Task::WithStack<defaultstacksize> task2{task2Handler,
                                                    &mutex}; // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
}

auto main() -> int
{
    SetupClocking();
    SEGGER_RTT_Init();

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_ALL);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_ALL);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ALL);

    LL_GPIO_AF_Remap_SWJ_NOJTAG();

    LL_GPIO_InitTypeDef gpioConfig;

    LL_GPIO_StructInit(&gpioConfig);

    gpioConfig.Pin = LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
    gpioConfig.Mode = LL_GPIO_MODE_OUTPUT;
    gpioConfig.OutputType = LL_GPIO_OUTPUT_PUSHPULL;

    LL_GPIO_Init(hal::GPIOC_Peripheral::Address(), &gpioConfig);

    gpioConfig.Pin = LL_GPIO_PIN_0;

    LL_GPIO_Init(hal::GPIOA_Peripheral::Address(), &gpioConfig);

    LL_SYSTICK_EnableIT();

    kernel::Scheduler kernel{mainThread};

    task1.priority = 0;
    task2.priority = 0;

    task1.name = "Task1";
    task2.name = "Task2";

    kernel::GetScheduler().readyTasksV2.push(task1.queueItemV2);
    kernel::GetScheduler().readyTasksV2.push(task2.queueItemV2);

    while (true)
    {
        // YieldTask();
    }

    return 0;
}