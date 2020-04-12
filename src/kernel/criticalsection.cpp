
#include "criticalsection.hpp"
#include "kernel/InterruptMasking.hpp"
#include "stm32f1xx.h"
#include <cassert>
#include <cstdint>

static volatile auto criticalNestingCounter = 0u;

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
        __BKPT();
    }
}