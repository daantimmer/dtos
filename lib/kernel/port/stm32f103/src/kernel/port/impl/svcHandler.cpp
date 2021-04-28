#include "kernel/scheduler.hpp"
#include "kernel/task.hpp"

namespace
{
    auto getStackPointer() -> void*
    {
        return currentTaskControlBlock->GetStack().GetStackPointer();
    }
}

extern "C" void __attribute__((naked)) SVC_Handler()
{
    asm volatile("bl %[schedulerSwitchContext]" ::[schedulerSwitchContext] "i"(getStackPointer));
    asm volatile("mov lr, r4");
    asm volatile("ldmia r0!, {r4-r11}");
    asm volatile("msr psp, r0");
    asm volatile("orr lr, #0xd");
    asm volatile("cpsie i");
    asm volatile("bx lr");
}