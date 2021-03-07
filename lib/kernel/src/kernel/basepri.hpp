
#pragma once

#include "stm32f1xx.h"
#include <cstdint>

namespace kernel
{
#if defined(CMAKE_RTOS_KERNEL_BASEPRI)
    constexpr auto RTOS_KERNEL_BASEPRI = CMAKE_RTOS_KERNEL_BASEPRI;
#else
    constexpr auto RTOS_KERNEL_BASEPRI = 0;
#endif

    constexpr auto GetBasePriorityRegisterValue()
    {
        constexpr auto basePriorityValue = RTOS_KERNEL_BASEPRI << (8 - __NVIC_PRIO_BITS);

        static_assert(RTOS_KERNEL_BASEPRI == 0 || (basePriorityValue > 0 && basePriorityValue <= UINT8_MAX),
                      "Invalid RTOS_KERNEL_BASEPRI");

        return basePriorityValue;
    }

    constexpr auto IsKernelPriorityHighest()
    {
        return RTOS_KERNEL_BASEPRI == 0;
    }
}