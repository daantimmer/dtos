
#include "kernel/port/interruptMasking.hpp"

#include "kernel/basepri.hpp"
#include "stm32f103xb.h"

#include <core_cm3.h>

auto kernel::port::DisableInterrupts() -> InterruptMask
{
    return DisableInterruptMasking();
}

auto kernel::port::EnableInterruptMasking() -> InterruptMask
{
    if constexpr (kernel::IsKernelPriorityHighest() == false)
    {
        const auto maskValue = __get_BASEPRI();
        constexpr auto basePriValue = kernel::GetBasePriorityRegisterValue();

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

auto kernel::port::DisableInterruptMasking() -> InterruptMask
{
    if constexpr (kernel::IsKernelPriorityHighest() == false)
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

auto kernel::port::EnableInterrupts(const InterruptMask maskValue) -> void
{
    RestoreInterruptMasking(maskValue);
}

auto kernel::port::RestoreInterruptMasking(const InterruptMask maskValue) -> void
{
    __set_BASEPRI(maskValue);
}