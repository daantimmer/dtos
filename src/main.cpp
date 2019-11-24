
// Rtos.cpp : Defines the entry point for the application.
//

#include "main.hpp"

#include "hal/port/peripherals.hpp"
#include "infra/List.hpp"
#include "interrupts.hpp"
#include "kernel/kernel.hpp"
#include "kernel/lockable.hpp"
#include "kernel/mainthread.hpp"
#include "kernel/port/systemclock.hpp"
#include "kernel/port/systemtick.hpp"
#include "kernel/scheduler.hpp"
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

auto task1Handler = [](Task&) {
    kernel::GetKernel().CurrentTask().RepeatEvery(std::chrono::milliseconds{250},
                                                  []() { LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13); });
};

auto task2Handler = [](Task&) {
    DelayTask(std::chrono::milliseconds{125});

    kernel::GetKernel().CurrentTask().RepeatEvery(std::chrono::milliseconds{250},
                                                  []() { LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13); });
};
}

auto RealDeal() -> int
{
    SetupClocking();

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

    kernel::MainThread mainThread{};
    kernel::Kernel kernel{mainThread};

    Task::WithStack<128> task1{task1Handler, {GPIOC, LL_GPIO_PIN_14}};
    Task::WithStack<128> task2{task2Handler, {GPIOC, LL_GPIO_PIN_15}};

    task1.priority = 0;
    task2.priority = 0;

    kernel::GetKernel().readyTasks.insert(task1);
    kernel::GetKernel().readyTasks.insert(task2);

    while (1)
    {
        // YieldTask();
    }

    return 0;
}