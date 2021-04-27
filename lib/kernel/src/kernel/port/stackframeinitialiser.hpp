#pragma once

#include "kernel/stacksize.hpp"
#include "kernel/status.hpp"
#include <cstddef>
#include <tuple>

namespace kernel
{
    struct RunnableTask;

    namespace port
    {
        std::uint32_t* InitialiseStack(std::uint32_t*, size_t);
        std::uint32_t* InitialiseStack(std::uint32_t*, StackSize_t);
        void SetEntry(kernel::RunnableTask*, std::uint32_t*);
    }
}