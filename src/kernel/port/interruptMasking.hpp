
#pragma once

#include "type_safe/strong_typedef.hpp"
#include <cstdint>

namespace kernel
{
    namespace port
    {
        struct InterruptMask_v1: type_safe::strong_typedef<InterruptMask_v1, std::uint32_t>
        {
            using type_safe::strong_typedef<InterruptMask_v1, std::uint32_t>::strong_typedef;
        };

        using InterruptMask = InterruptMask_v1;

        auto EnableInterruptMasking() -> InterruptMask;
        auto DisableInterruptMasking() -> InterruptMask;

        auto RestoreInterruptMasking() -> void;
        auto RestoreInterruptMasking(InterruptMask maskValue) -> void;
    }
}