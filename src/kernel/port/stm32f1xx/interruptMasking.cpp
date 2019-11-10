
#include "kernel/port/interruptMasking.hpp"

#include "stm32f103xb.h"

#include <core_cm3.h>

auto rtos::port::DisableInterrupts() -> InterruptMask
{
    return DisableInterruptMasking();
}

auto rtos::port::EnableInterruptMasking() -> InterruptMask
{
    const auto maskValue = __get_BASEPRI();

    __set_BASEPRI(0);

    return maskValue;
}

auto rtos::port::DisableInterruptMasking() -> InterruptMask
{
    const auto maskValue = __get_BASEPRI();

    __set_BASEPRI(0);

    return maskValue;
}

auto rtos::port::EnableInterrupts(const InterruptMask maskValue) -> void
{
    RestoreInterruptMasking(maskValue);
}

auto rtos::port::RestoreInterruptMasking(const InterruptMask maskValue) -> void
{
    __set_BASEPRI(maskValue);
}