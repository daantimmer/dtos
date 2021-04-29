#ifndef SYNCHRONISATION_MUTEX_HPP
#define SYNCHRONISATION_MUTEX_HPP

#include "kernel/status.hpp"
#include "kernel/taskList.hpp"

namespace kernel
{
    struct MutexControlBlock
    {
    protected:
        MutexControlBlock();

        MutexControlBlock(const MutexControlBlock&) = delete;
        MutexControlBlock(MutexControlBlock&&) = delete;

        MutexControlBlock& operator=(const MutexControlBlock&) = delete;
        MutexControlBlock& operator=(MutexControlBlock&&) = delete;

        StatusCode DoBlock();

        void DoLock();
        void DoUnlock();

        TaskList<> blockedList;
        RunnableTask* owner;
    };

    struct Mutex: private MutexControlBlock
    {
        Mutex();
        ~Mutex();

        StatusCode Lock();

        bool TryLock();
        bool TryLockFor();

        StatusCode Unlock();

    private:
        StatusCode InternalTryLock();
    };
}

#endif
