
#include "criticalsection.hpp"

#include "kernel/InterruptMasking.hpp"
// #include "utils.hpp"

#include <cassert>
#include <cstdint>

static volatile uint32_t criticalNestingCounter = 0;

void EnterCriticalSection()
{
    // DisableInterrupts();
    rtos::port::DisableInterruptMasking();

    criticalNestingCounter++;
}

void ExitCriticalSection()
{
    if (criticalNestingCounter > 0)
    {
        criticalNestingCounter--;

        if (criticalNestingCounter == 0)
        {
            // EnableInterrupts();
            rtos::port::RestoreInterruptMasking();
        }
    }
    else
    {
        assert(false);
    }
}