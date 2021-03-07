#include "kernel/port/triggerTaskSwitch.hpp"
#include "stm32f103xb.h"

void TriggerTaskSwitch()
{
    // Ignore 'do not use c-style cast' warning. Can't be avoided here
    // NOLINTNEXTLINE
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    asm volatile("isb");
    asm volatile("dsb" ::: "memory");
}