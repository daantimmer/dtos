
#include "kernel/port/interruptMasking.hpp"
#include "kernel/basepri.hpp"
#include "stm32f1xx.h"

#if defined(CLANG_TIDY)
namespace
{
    constexpr std::uint32_t __get_BASEPRI()
    {
        return 0;
    }

    constexpr void __set_BASEPRI(std::uint32_t /*unused*/)
    {}
}
#endif

auto kernel::port::EnableInterruptMasking() -> InterruptMask
{
    if constexpr (!kernel::IsKernelPriorityHighest())
    {
        const auto maskValue = __get_BASEPRI();
        constexpr auto basePriValue = kernel::GetBasePriorityRegisterValue();

        __set_BASEPRI(basePriValue);

        return InterruptMask{maskValue};
    }
    else
    {
        const auto maskValue = __get_PRIMASK();

        __disable_irq();

        return InterruptMask{maskValue};
    }
}

auto kernel::port::DisableInterruptMasking() -> InterruptMask
{
    if constexpr (!kernel::IsKernelPriorityHighest())
    {
        const auto maskValue = __get_BASEPRI();

        __set_BASEPRI(0);

        return InterruptMask{maskValue};
    }
    else
    {
        const auto maskValue = __get_PRIMASK();

        __enable_irq();

        return InterruptMask{maskValue};
    }
}

auto kernel::port::RestoreInterruptMasking() -> void
{
    RestoreInterruptMasking(InterruptMask{std::uint32_t{0}});
}

auto kernel::port::RestoreInterruptMasking(const InterruptMask maskValue) -> void
{
    if constexpr (!kernel::IsKernelPriorityHighest())
    {
        __set_BASEPRI(static_cast<std::uint32_t>(maskValue));
    }
    else
    {
        __set_PRIMASK(static_cast<std::uint32_t>(maskValue));
    }
}