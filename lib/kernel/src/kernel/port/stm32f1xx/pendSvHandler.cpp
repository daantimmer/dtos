
// #if false /* To Be used*/

#include "kernel/pendSvHandler.hpp"
#include "kernel/basepri.hpp"
#include "kernel/scheduler.hpp"
#include "kernel/task.hpp"

static auto schedulerSwitchContextWrapper(void* const ptr) -> void*
{
    currentTaskControlBlock->SetStackPointer(ptr);
    TaskScheduler();
    return currentTaskControlBlock->GetStackPointer();
}

extern "C" __attribute__((naked)) void PendSV_Handler()
{
#ifndef CLANG_TIDY
    if constexpr (!kernel::IsKernelPriorityHighest())
    {
        asm volatile("mov r0, %[basePriority]" ::[basePriority] "i"(kernel::GetBasePriorityRegisterValue()));
        asm volatile("msr basepri, r0");
    }
    else
    {
        asm volatile("cpsid i");
    }

    if constexpr (false /* ARMv6*/)
    {
    }
    else
    {
        asm volatile("mrs r0, psp");
        asm volatile("stmdb r0!, {r4-r11}");
        asm volatile("mov r4, lr");
        asm volatile("bl %[schedulerSwitchContext]" ::[schedulerSwitchContext] "i"(schedulerSwitchContextWrapper));
        asm volatile("mov lr, r4");
        asm volatile("ldmia r0!, {r4-r11}");
        asm volatile("msr psp, r0");
    }

    if constexpr (!kernel::IsKernelPriorityHighest())
    {
        asm volatile("mov r0, #0");
        asm volatile("msr basepri, r0");
    }
    else
    {
        asm volatile("cpsie i");
    }

    asm volatile("bx lr");
    asm volatile(".ltorg");

    __builtin_unreachable();
#endif
}

// #endif