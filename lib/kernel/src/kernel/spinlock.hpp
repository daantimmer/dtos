
#pragma once

#include "criticalsection.hpp"

namespace kernel
{
struct SpinLock
{
    SpinLock()
    {
        __sync_lock_release(&flag);
    }

    void lock()
    {
        Lock();
    }

    void try_lock()
    {
        TryLock();
    }

    void unlock()
    {
        return Unlock();
    }

    void Lock()
    {
        while (__sync_lock_test_and_set(&flag, 1u))
        {
            while (flag)
            {
            }
        }
    }

    bool TryLock()
    {
        return __sync_lock_test_and_set(&flag, 1u) == 0u;
    }

    void Unlock()
    {
        __sync_lock_release(&flag);
    }

protected:
    volatile std::uint32_t flag{};
};

using TheBottleneck = SpinLock;
}
