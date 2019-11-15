#pragma once

#include "kernel/task.hpp"

#include <cstdint>

namespace kernel
{
struct MainThread : TaskStack
{
    MainThread();
    void Run() final;
};

}