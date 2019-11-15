#include "kernel/mainthread.hpp"

#include <stm32f1xx.h>

extern std::uint32_t __processStack_end[];

kernel::MainThread::MainThread()
    : TaskStack {&__processStack_end[0]}
{
}

void kernel::MainThread::Run()
{
    while (1)
    {
        __BKPT();
    }
}