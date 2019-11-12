#pragma once

#include "kernel/port/interruptMasking.hpp"

namespace kernel
{
template<kernel::port::InterruptMask (&Function)()>
struct InterruptMaskingUnmasking
{
public:
    InterruptMaskingUnmasking()
        : interruptMask{Function()}
    {
    }

    ~InterruptMaskingUnmasking()
    {
        kernel::port::RestoreInterruptMasking(interruptMask);
    }

    InterruptMaskingUnmasking(const InterruptMaskingUnmasking&) = delete;
    InterruptMaskingUnmasking(InterruptMaskingUnmasking&&) = delete;

    InterruptMaskingUnmasking& operator=(const InterruptMaskingUnmasking&) = delete;
    InterruptMaskingUnmasking& operator=(InterruptMaskingUnmasking&&) = delete;

private:
    kernel::port::InterruptMask interruptMask;
};

using InterruptUnmasking = InterruptMaskingUnmasking<kernel::port::DisableInterruptMasking>;
using InterruptMasking = InterruptMaskingUnmasking<kernel::port::EnableInterruptMasking>;
}