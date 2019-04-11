
// Rtos.cpp : Defines the entry point for the application.
//


#include <cstdint>
#include <cstdint>

#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_utils.h"

#include "interrupts.hpp"

#include "infra/List.hpp"
#include "os/scheduler.hpp"
#include "os/lockable.hpp"

void SystemClock_Config()
{
    LL_UTILS_PLLInitTypeDef pllInitTypeDef{ LL_RCC_PLL_MUL_6, LL_RCC_PREDIV_DIV_1 };
    LL_UTILS_ClkInitTypeDef clkInitTypeDef{ LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_2, LL_RCC_APB2_DIV_1 };

    LL_PLL_ConfigSystemClock_HSE(
        8000000U,
        LL_UTILS_HSEBYPASS_OFF,
        &pllInitTypeDef,
        &clkInitTypeDef
    );

    LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_PLL);
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);

    LL_RCC_ClocksTypeDef RCC_Clocks;
    LL_RCC_GetSystemClocksFreq(&RCC_Clocks);

    LL_InitTick(RCC_Clocks.HCLK_Frequency, 1000 );
}

extern "C" int RealDeal()
{
    std::int32_t i = 10;

    SystemClock_Config();

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

    while (1){}

    return i;
}