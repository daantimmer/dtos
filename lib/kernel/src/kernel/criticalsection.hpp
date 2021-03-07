
#pragma once

void EnterCriticalSection();
void ExitCriticalSection();

struct ScopedCriticalSection
{
    ScopedCriticalSection()
    {
        EnterCriticalSection();
    }

    ~ScopedCriticalSection()
    {
        ExitCriticalSection();
    }
};