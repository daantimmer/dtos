
#include "criticalsection.hpp"
#include "kernel/interruptMasking.hpp"
#include "stm32f1xx.h"
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
        __BKPT();
    }
}