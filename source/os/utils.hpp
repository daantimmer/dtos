
#pragma once

#include <cstdint>

inline __attribute__((always_inline)) void DisableInterrupts()
{
    uint32_t scratch;

    asm volatile("mov %0, %1\n"
                 "msr basepri, %0\n"
                 "isb\n"
                 "dsb\n"
                 : "=r"(scratch)
                 : "i"(1)
                 : "memory");
}

inline __attribute__((always_inline)) void EnableInterrupts(uint32_t maskValue = 0)
{
    asm volatile("msr basepri, %0" ::"r"(maskValue) : "memory");
}
