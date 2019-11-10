#pragma once

#include "kernel/port/interruptMasking.hpp"

namespace rtos
{
template<rtos::port::InterruptMask (&Function)()>
struct InterruptMaskingUnmasking
{
public:
    InterruptMaskingUnmasking()
        : interruptMask{Function()}
    {
    }

    ~InterruptMaskingUnmasking()
    {
        rtos::port::RestoreInterruptMasking(interruptMask);
    }

    InterruptMaskingUnmasking(const InterruptMaskingUnmasking&) = delete;
    InterruptMaskingUnmasking(InterruptMaskingUnmasking&&) = delete;

    InterruptMaskingUnmasking& operator=(const InterruptMaskingUnmasking&) = delete;
    InterruptMaskingUnmasking& operator=(InterruptMaskingUnmasking&&) = delete;

private:
    rtos::port::InterruptMask interruptMask;
};

using InterruptUnmasking = InterruptMaskingUnmasking<rtos::port::DisableInterruptMasking>;
using InterruptMasking = InterruptMaskingUnmasking<rtos::port::EnableInterruptMasking>;
}