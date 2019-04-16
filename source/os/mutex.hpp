
#pragma once

#include "criticalsection.hpp"
#include "scheduler.hpp"

struct Mutex
{
    struct Resource
    {
        friend Mutex;

    protected:
        volatile Mutex* owner = nullptr;
    };

    Mutex(Mutex::Resource& obj)
        : obj(obj)
    {
    }

    void Acquire()
    {
        do
        {
            EnterCriticalSection();

            if (obj.owner != this)
            {
                obj.owner = this;

				ExitCriticalSection();
            }
			else
			{
                ExitCriticalSection();

                YieldTask();
			}

        } while (obj.owner != this);
    }

    void Release()
    {
        EnterCriticalSection();

        obj.owner = nullptr;

        ExitCriticalSection();
    }

private:
    Mutex::Resource& obj;
};

using TheBottleneck = Mutex;
