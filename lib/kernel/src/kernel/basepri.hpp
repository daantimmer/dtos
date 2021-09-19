
#pragma once

#include "stm32f1xx.h"
#include <cstdint>

#if !defined(CMAKE_RTOS_KERNEL_BASEPRI)
#define CMAKE_RTOS_KERNEL_BASEPRI 0
#endif

#define IS_KERNEL_PRIORITY_HIGHEST() (CMAKE_RTOS_KERNEL_BASEPRI == 0)

namespace kernel
{
    constexpr auto RTOS_KERNEL_BASEPRI = CMAKE_RTOS_KERNEL_BASEPRI;

    constexpr auto GetBasePriorityRegisterValue()
    {
        constexpr auto basePriorityValue = RTOS_KERNEL_BASEPRI << (8 - __NVIC_PRIO_BITS);

        static_assert(RTOS_KERNEL_BASEPRI == 0 || (basePriorityValue > 0 && basePriorityValue <= UINT8_MAX),
                      "Invalid RTOS_KERNEL_BASEPRI");

        return basePriorityValue;
    }

    constexpr auto IsKernelPriorityHighest()
    {
        return IS_KERNEL_PRIORITY_HIGHEST();
    }
}