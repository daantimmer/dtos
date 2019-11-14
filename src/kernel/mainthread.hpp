#pragma once

#include "kernel/task.hpp"

#include <cstdint>

extern std::uint32_t __processStack_end[];

namespace kernel
{
struct MainThread : TaskStack
{
    MainThread()
        : TaskStack {&__processStack_end[0]}
    {
    }

    virtual void Run() final
    {
        while (1)
        {
        }
    }
};

}