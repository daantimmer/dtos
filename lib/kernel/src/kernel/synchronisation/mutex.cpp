#include "kernel/synchronisation/mutex.hpp"
#include "kernel/getkernel.hpp"
#include "kernel/interruptMasking.hpp"
#include "kernel/scheduler.hpp"

kernel::MutexControlBlock::MutexControlBlock()
{}

kernel::StatusCode kernel::MutexControlBlock::DoBlock()
{
    return kernel::GetScheduler().Block(blockedList);
}

void kernel::MutexControlBlock::DoLock()
{
    auto& scheduler = kernel::GetScheduler();
    owner = &scheduler.CurrentTask();
}

void kernel::MutexControlBlock::DoUnlock()
{
    if (blockedList.empty())
    {
        owner = nullptr;
        return;
    }

    owner = blockedList.front().task;
    kernel::GetScheduler().Unblock(*owner);
}

kernel::Mutex::Mutex()
{}

kernel::Mutex::~Mutex()
{}

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

bool kernel::Mutex::TryLock()
{
    return false;
}

bool kernel::Mutex::TryLockFor()
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