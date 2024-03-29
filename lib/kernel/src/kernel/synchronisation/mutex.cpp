#include "kernel/synchronisation/mutex.hpp"
#include "kernel/getkernel.hpp"
#include "kernel/interruptMasking.hpp"
#include "kernel/scheduler.hpp"

kernel::StatusCode kernel::MutexControlBlock::DoBlock()
{
    return kernel::GetScheduler().Block(blockedList);
}

void kernel::MutexControlBlock::DoLock()
{
    owner = &kernel::Scheduler::CurrentTask();
}

void kernel::MutexControlBlock::DoUnlock()
{
    if (blockedList.empty())
    {
        owner = nullptr;
        return;
    }

    auto& ctrlBlock = blockedList.front();
    owner = &ctrlBlock.Owner();
    kernel::GetScheduler().Unblock(ctrlBlock);
}

kernel::StatusCode kernel::Mutex::Lock()
{
    const kernel::InterruptMasking interruptMasking{};

    auto status = kernel::StatusCode::Undefined;

    do
    {
        if (status = InternalTryLock(); status == kernel::StatusCode::Busy)
        {
            status = DoBlock();
        }
        else
        {
            return status;
        }

    } while (status == kernel::StatusCode::Interrupted);

    return kernel::StatusCode::Undefined;
}

bool kernel::Mutex::TryLock() // NOLINT
{
    return false;
}

bool kernel::Mutex::TryLockFor() // NOLINT
{
    return false;
}

kernel::StatusCode kernel::Mutex::Unlock()
{
    const kernel::InterruptMasking interruptMasking{};

    DoUnlock();

    return kernel::StatusCode::Ok;
}

kernel::StatusCode kernel::Mutex::InternalTryLock()
{
    if (owner == nullptr)
    {
        DoLock();
        return kernel::StatusCode::Ok;
    }

    return kernel::StatusCode::Busy;
}