#ifndef SYNCHRONISATION_SEMAPHORE_HPP
#define SYNCHRONISATION_SEMAPHORE_HPP

#include "kernel/status.hpp"
#include "kernel/taskList.hpp"
#include <cstdlib>

namespace kernel
{
    struct Semaphore
    {
        Semaphore(std::size_t initialCounters = 0);

        void Release(std::size_t count = 1);

        kernel::StatusCode Acquire();
        kernel::StatusCode TryAcquire();

        std::size_t counter;

    private:
        void InternalRelease();
        kernel::StatusCode InternalTryAcquire();
        kernel::StatusCode InternalBlock();

        TaskList<> blockedList;
    };
}

#endif
