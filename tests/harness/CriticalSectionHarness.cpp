#include "CriticalSectionHarness.hpp"

CriticalSectionSpy* CriticalSectionSpy::instance = nullptr;

void EnterCriticalSection()
{
	if (CriticalSectionSpy::instance != nullptr)
	{
        CriticalSectionSpy::instance->Enter();
	}
}

void ExitCriticalSection()
{
    if (CriticalSectionSpy::instance != nullptr)
    {
        CriticalSectionSpy::instance->Exit();
    }
}