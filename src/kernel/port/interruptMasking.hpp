
#pragma once

#include <cstdint>

namespace kernel
{
namespace port
{
using InterruptMask = std::uint32_t;

[[deprecated("Replaced by DisableInterruptMasking")]] auto DisableInterrupts() -> InterruptMask;

auto EnableInterruptMasking() -> InterruptMask;
auto DisableInterruptMasking() -> InterruptMask;

[[deprecated("Replaced by RestoreInterruptMasking")]] auto EnableInterrupts(const InterruptMask maskValue = 0) -> void;
auto RestoreInterruptMasking(const InterruptMask maskValue = 0) -> void;
}
}