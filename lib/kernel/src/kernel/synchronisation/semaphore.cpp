#include "kernel/synchronisation/semaphore.hpp"
#include "kernel/getkernel.hpp"
#include "kernel/interruptMasking.hpp"
#include "kernel/scheduler.hpp"

kernel::Semaphore::Semaphore(std::size_t initialCounter, std::size_t maxCounter)
    : counter{initialCounter}
    , maxCounter{maxCounter}
{}

void kernel::Semaphore::Release(std::size_t count)
{
    const InterruptMasking interruptMasking;

    for (; count > 0; --count)
    {
        InternalRelease();
    }
}

kernel::StatusCode kernel::Semaphore::Acquire()
{
    const InterruptMasking interruptMasking;

    const auto status = InternalTryAcquire();

    if (status != kernel::StatusCode::Busy)
    {
        return status;
    }

    return InternalBlock();
}

kernel::StatusCode kernel::Semaphore::TryAcquire()
{
    const InterruptMasking interruptMasking;

    return InternalTryAcquire();
}

void kernel::Semaphore::InternalRelease()
{
    if (counter == maxCounter)
    {
        return;
    }

    if (blockedList.empty() == false)
    {
        auto& scheduler = kernel::GetScheduler();
        auto& ctrlBlock = blockedList.front();

        scheduler.Unblock(ctrlBlock);

        return;
    }

    ++counter;
}

kernel::StatusCode kernel::Semaphore::InternalTryAcquire()
{
    if (counter == 0)
    {
        return kernel::StatusCode::Busy;
    }

    --counter;

    return kernel::StatusCode::Ok;
}

kernel::StatusCode kernel::Semaphore::InternalBlock()
{
    auto& scheduler = kernel::GetScheduler();
    scheduler.Block(blockedList);

    return kernel::StatusCode::Ok;
}