
// #include "kernel/port/systemclock.hpp"

// #include "stm32f1xx.h"
// #include "stm32f1xx_ll_rcc.h"
// #include "stm32f1xx_ll_utils.h"

// void SystemClock_Setup()
// {
//     LL_UTILS_PLLInitTypeDef pllInitTypeDef{LL_RCC_PLL_MUL_6, LL_RCC_PREDIV_DIV_1};
//     LL_UTILS_ClkInitTypeDef clkInitTypeDef{LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_2, LL_RCC_APB2_DIV_1};

//     LL_PLL_ConfigSystemClock_HSE(8000000U, LL_UTILS_HSEBYPASS_OFF, &pllInitTypeDef, &clkInitTypeDef);

//     LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_PLL);
//     LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
// }