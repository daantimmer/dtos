#include "kernel/port/triggerTaskSwitch.hpp"
#include "kernel/interruptMasking.hpp"

void ForceContextSwitch()
{
    TriggerTaskSwitch();
    const kernel::InterruptUnmasking interruptUnmasking;
}