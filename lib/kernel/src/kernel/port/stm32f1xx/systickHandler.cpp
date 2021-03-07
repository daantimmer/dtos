#include "kernel/interruptMasking.hpp"
#include "kernel/port/triggerTaskSwitch.hpp"
#include "kernel/scheduler.hpp"

extern "C" void SysTick_Handler()
{
    kernel::InterruptUnmasking interruptUnmasking;

    if (SchedulerTick())
    {
        TriggerTaskSwitch();
    }
}