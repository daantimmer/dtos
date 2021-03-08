#include "kernel/scheduler.hpp"
#include "kernel/task.hpp"

namespace
{
    auto getStackPointer() -> void*
    {
        return currentTaskControlBlock->GetStackPointer();
    }
}

extern "C" void __attribute__((naked)) SVC_Handler()
{}