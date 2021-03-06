#include "kernel/mainthread.hpp"
#include "stm32f1xx.h"

// __processStack_end is provided by the linker script and does not map 1:1 to std::array therefor block
// cppcoreguidelines-avoid-c-arrays
// NOLINTNEXTLINE
extern std::uint32_t __processStack_end[];

kernel::MainThread::MainThread()
    : TaskStack{&__processStack_end[0]}
{}

void kernel::MainThread::Run()
{
    while (true)
    {
        __BKPT();
    }
}