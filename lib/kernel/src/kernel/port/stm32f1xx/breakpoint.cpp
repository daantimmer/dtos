#include "kernel/port/breakpoint.hpp"

void kernel::port::breakpoint()
{
    asm volatile("bkpt");
}
