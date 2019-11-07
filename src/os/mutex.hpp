
#pragma once

#include "criticalsection.hpp"
#include "scheduler.hpp"

struct Mutex
{
	void Lock()
	{
		while (TryLock() == false)
		{
            BlockTask();
		}
	}

	bool TryLock()
	{
        ScopedCriticalSection scopedCriticalSection;

		if (isLocked == true)
		{
            return false;
		}
		else
		{
            isLocked = true;

            return true;
		}
	}

	void Unlock()
	{
        ScopedCriticalSection scopedCriticalSection;

		isLocked = false;
	}

protected:
    volatile bool isLocked;
};

struct ScopedLock
{
    ScopedLock(Mutex& mutex)
        : mutex(mutex)
    {
        mutex.Lock();
    }

    ~ScopedLock()
    {
        mutex.Unlock();
    }

protected:
    Mutex& mutex;
};

using TheBottleneck = Mutex;
