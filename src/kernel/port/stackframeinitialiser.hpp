#pragma once

#include "kernel/stacksize.hpp"
#include "kernel/status.hpp"
#include <cstddef>
#include <tuple>

namespace kernel
{
    namespace port
    {
        auto InitialiseStack(void* entry, std::uint32_t*, StackSize_t) -> kernel::Status<std::uint32_t*>;
    }
}