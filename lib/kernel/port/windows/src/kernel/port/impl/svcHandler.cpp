#include "kernel/getkernel.hpp"
#include "kernel/scheduler.hpp"
#include "kernel/task.hpp"

namespace
{
    auto getStackPointer() -> void*
    {
        return kernel::GetScheduler().CurrentTaskControlBlock().GetStack().GetStackPointer();
    }
}

extern "C" void __attribute__((naked)) SVC_Handler()
{}