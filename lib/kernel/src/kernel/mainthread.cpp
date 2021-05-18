#include "kernel/mainthread.hpp"
#include "stm32f1xx.h"

// __processStack_start and _end is provided by the linker script
// and does not map 1:1 to std::array therefor
// block modernize-avoid-c-arrays and cppcoreguidelines-avoid-c-arrays
extern std::uint32_t __processStack_start[]; // NOLINT(modernize-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays)
extern std::uint32_t __processStack_end[]; // NOLINT(modernize-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays)

kernel::MainThread::MainThread()
    : TaskBase{{&__processStack_end[0], static_cast<std::size_t>(&__processStack_end[0] - &__processStack_start[0])},
               "MainThread"}
{}

void kernel::MainThread::Run()
{
    while (true)
    {
        __BKPT();
    }
}

// bool kernel::MainThread::StackSafe() const
// {
//     return true;
// }

// std::size_t kernel::MainThread::StackAvailable()
// {
//     return 0;
// }