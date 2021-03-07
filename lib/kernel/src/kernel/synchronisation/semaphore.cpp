#include "kernel/synchronisation/semaphore.hpp"
#include "kernel/getkernel.hpp"
#include "kernel/interruptMasking.hpp"
#include "kernel/scheduler.hpp"

kernel::Semaphore::Semaphore(std::size_t initialCounter)
    : counter(initialCounter)
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
    const kernel::InterruptMasking interruptMasking{};

    auto status = kernel::StatusCode::Undefined;

    do
    {
        if (status = InternalTryAcquire(); status == kernel::StatusCode::Busy)
        {
            status = InternalBlock();
        }
        else
        {
            return status;
        }

    } while (status == kernel::StatusCode::Interrupted);

    return kernel::StatusCode::Undefined;
}

kernel::StatusCode kernel::Semaphore::TryAcquire()
{
    const InterruptMasking interruptMasking;

    return InternalTryAcquire();
}

void kernel::Semaphore::InternalRelease()
{
    if (blockedList.empty() == true)
    {
        ++counter;
    }

    auto& scheduler = kernel::GetScheduler();
    auto& task = *blockedList.front().task;
    scheduler.Unblock(task);
}

kernel::StatusCode kernel::Semaphore::InternalTryAcquire()
{
    if (counter > 0)
    {
        --counter;

        return kernel::StatusCode::Ok;
    }

    return kernel::StatusCode::Busy;
}

kernel::StatusCode kernel::Semaphore::InternalBlock()
{
    auto& scheduler = kernel::GetScheduler();
    scheduler.Block(blockedList);

    return kernel::StatusCode::Undefined;
}