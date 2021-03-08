#ifndef KERNEL_CRITICALSECTION_HPP
#define KERNEL_CRITICALSECTION_HPP

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

#endif
