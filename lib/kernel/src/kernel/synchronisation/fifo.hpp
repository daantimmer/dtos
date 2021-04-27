#ifndef SYNCHRONISATION_FIFO_HPP
#define SYNCHRONISATION_FIFO_HPP

#include "infra/util/Function.hpp"
#include "kernel/interruptMasking.hpp"
#include "kernel/status.hpp"
#include "kernel/synchronisation/semaphore.hpp"
#include <algorithm>

namespace kernel
{
    struct FifoBase
    {
        using WaitOnSemaphoreFunction = infra::Function<kernel::StatusCode(kernel::Semaphore&)>;
        using AccessQueueFunction = infra::Function<void(void*&)>;

        FifoBase() = default;
        FifoBase(void* const buffer, std::size_t size, std::size_t count)
            : storageStart{buffer}
            , storageEnd{static_cast<std::uint8_t*>(buffer) + (size * count)}
            , pushSemaphore{count, count}
            , popSemaphore{0, count}
        {}

        ~FifoBase() = default;

        kernel::StatusCode Push(const WaitOnSemaphoreFunction& waitOnSemaphoreFunction,
                                const AccessQueueFunction& accessQueueFunction)
        {
            return PushPop(waitOnSemaphoreFunction, accessQueueFunction, pushSemaphore, popSemaphore, storageStart);
        }

        kernel::StatusCode Pop(const WaitOnSemaphoreFunction& waitOnSemaphoreFunction,
                               const AccessQueueFunction& accessQueueFunction)
        {
            return PushPop(waitOnSemaphoreFunction, accessQueueFunction, popSemaphore, pushSemaphore, storageStart);
        }

    private:
        kernel::StatusCode PushPop(const WaitOnSemaphoreFunction& waitOnSemaphoreFunction,
                                   const AccessQueueFunction& accessQueueFunction,
                                   kernel::Semaphore& waitBeforeAccessSemaphore,
                                   kernel::Semaphore& releaseAfterAccessSemaphore,
                                   void*& storage)
        {
            const InterruptMasking interruptMasking;

            if (const auto ret = waitOnSemaphoreFunction(waitBeforeAccessSemaphore); ret != kernel::StatusCode::Ok)
            {
                return ret;
            }

            accessQueueFunction(storage);

            storage = static_cast<std::uint8_t*>(storage) + 1;

            if (storage == storageEnd)
            {
                storage = storageStart;
            }

            releaseAfterAccessSemaphore.Release();
            return kernel::StatusCode::Ok;
        }

        void* storageStart{nullptr};
        void* storageEnd{nullptr};

        void* readPointer{nullptr};
        void* writePointer{nullptr};

        kernel::Semaphore pushSemaphore;
        kernel::Semaphore popSemaphore;
    };

    struct Fifo
    {};
}

#endif
