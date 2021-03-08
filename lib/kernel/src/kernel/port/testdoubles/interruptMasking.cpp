
#include "kernel/port/interruptMasking.hpp"

auto kernel::port::EnableInterruptMasking() -> InterruptMask
{
    return InterruptMask{0u};
}

auto kernel::port::DisableInterruptMasking() -> InterruptMask
{
    return InterruptMask{0u};
}

auto kernel::port::RestoreInterruptMasking() -> void
{
    RestoreInterruptMasking(InterruptMask{0u});
}

auto kernel::port::RestoreInterruptMasking(const InterruptMask) -> void
{}