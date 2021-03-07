#include "kernel/port/systemtick.hpp"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_utils.h"
#include <chrono>
#include <ratio>

namespace systemtick
{
    void Setup()
    {
        LL_RCC_ClocksTypeDef RCC_Clocks;
        LL_RCC_GetSystemClocksFreq(&RCC_Clocks);

        LL_InitTick(RCC_Clocks.HCLK_Frequency, tickfrequency);
    }

} // namespace systemtick