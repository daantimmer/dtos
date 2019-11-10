
#pragma once

#include "stm32f103xb.h"
#include <core_cm3.h>
#include <cstdint>

inline std::uint32_t DisableInterrupts()
{
    const auto mask = __get_BASEPRI();
    __set_BASEPRI(0);
    return mask;
}

inline void EnableInterrupts(uint32_t maskValue = 0)
{
    __set_BASEPRI(maskValue);
}
