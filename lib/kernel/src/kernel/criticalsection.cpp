
#include "criticalsection.hpp"
#include "kernel/interruptMasking.hpp"
#include "kernel/port/breakpoint.hpp"
#include <cassert>
#include <cstdint>

static volatile auto criticalNestingCounter = 0U;

void EnterCriticalSection()
{
    kernel::port::DisableInterruptMasking();

    ++criticalNestingCounter;
}

void ExitCriticalSection()
{
    if (criticalNestingCounter > 0)
    {
        --criticalNestingCounter;

        if (criticalNestingCounter == 0)
        {
            kernel::port::RestoreInterruptMasking();
        }
    }
    else
    {
        kernel::port::breakpoint();
    }
}