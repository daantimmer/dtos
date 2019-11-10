
// Rtos.cpp : Defines the entry point for the application.
//

#include "main.hpp"

#include "elib/register.hpp"
#include "infra/List.hpp"
#include "interrupts.hpp"
#include "kernel/lockable.hpp"
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

void SetupClocking()
{
    SystemClock_Setup();
    systemtick::Setup();
}

extern "C" int RealDeal()
{
    (elib::memory::Instance().data.regADC1)->JOFR2 = 5;

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

    LL_GPIO_Init(GPIOC, &gpioConfig);

    gpioConfig.Pin = LL_GPIO_PIN_0;

    LL_GPIO_Init(GPIOA, &gpioConfig);

    LL_SYSTICK_EnableIT();

    startFirstTask();

    while (1)
    {
    }

    return 0;
}