#pragma once

#include "kernel/status.hpp"

#include <cstddef>
#include <tuple>

namespace kernel
{
namespace port
{
kernel::Status<std::uint32_t*> InitialiseStack(void* entry, std::uint32_t*, std::size_t);
}
}