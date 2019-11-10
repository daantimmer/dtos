
#include "kernel/port/interruptMasking.hpp"

#include "stm32f103xb.h"

#include <core_cm3.h>

constexpr auto RTOS_PORT_BASEPRI = 1;

auto rtos::port::DisableInterrupts() -> InterruptMask
{
    return DisableInterruptMasking();
}

auto rtos::port::EnableInterruptMasking() -> InterruptMask
{
    if constexpr (RTOS_PORT_BASEPRI >= 0)
    {
        const auto maskValue = __get_BASEPRI();
        const auto basePriValue = RTOS_PORT_BASEPRI << (8 - __NVIC_PRIO_BITS);

        __set_BASEPRI(basePriValue);

        return maskValue;
    }
    else
    {
        const auto maskValue = __get_PRIMASK();

        __disable_irq();

        return maskValue;
    }
}

auto rtos::port::DisableInterruptMasking() -> InterruptMask
{
    if constexpr (RTOS_PORT_BASEPRI >= 0)
    {
        const auto maskValue = __get_BASEPRI();

        __set_BASEPRI(0);

        return maskValue;
    }
    else
    {
        const auto maskValue = __get_PRIMASK();

        __enable_irq();

        return maskValue;
    }
}

auto rtos::port::EnableInterrupts(const InterruptMask maskValue) -> void
{
    RestoreInterruptMasking(maskValue);
}

auto rtos::port::RestoreInterruptMasking(const InterruptMask maskValue) -> void
{
    __set_BASEPRI(maskValue);
}