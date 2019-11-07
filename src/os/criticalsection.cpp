
#include "criticalsection.hpp"

#include "utils.hpp"

#include <cstdint>
#include <cassert>

static volatile uint32_t criticalNestingCounter = 0;

void EnterCriticalSection()
{
    DisableInterrupts();

    criticalNestingCounter++;
}

void ExitCriticalSection()
{
    if (criticalNestingCounter > 0)
    {
        criticalNestingCounter--;

        if (criticalNestingCounter == 0)
        {
            EnableInterrupts();
        }
    }
    else
    {
        assert(false);
    }
}