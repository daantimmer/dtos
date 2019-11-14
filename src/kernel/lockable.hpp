#pragma once

#include "infra/List.hpp"
#include "kernel/InterruptMasking.hpp"
#include "scheduler.hpp"

#include <atomic>

struct Lockable
{
    void Lock()
    {
        kernel::port::DisableInterruptMasking();

        void* owner = lock;

        while (owner != nullptr)
        {
            blockedList.AddBack(currentTaskControlBlock->blockedItem);

            kernel::port::RestoreInterruptMasking();

            //blockTask();

            kernel::port::DisableInterruptMasking();
            owner = lock;
        }

        lock = currentTaskControlBlock;

        kernel::port::RestoreInterruptMasking();
    }

    void Unlock()
    {
        kernel::port::DisableInterruptMasking();

        if (lock == currentTaskControlBlock)
        {
            lock = nullptr;

            while (blockedList.Empty() == false)
            {
                //auto task = blockedList.PeekFront();
                blockedList.PopFront();

                //unblockTask(task);
            }
        }

        kernel::port::RestoreInterruptMasking();
    }

    void* lock = nullptr;
    List<RunnableTask> blockedList;
};