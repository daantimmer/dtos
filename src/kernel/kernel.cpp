#include "kernel/kernel.hpp"

#include "kernel/scheduler.hpp"
#include "kernel/mainthread.hpp"

kernel::Kernel::Kernel(MainThread& mainThread)
{
    currentTaskControlBlock = &mainThread;
}