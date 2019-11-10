#pragma once

#include "infra/List.hpp"
#include "kernel/InterruptMasking.hpp"
#include "scheduler.hpp"
// #include "utils.hpp"

#include <atomic>

struct Lockable
{
    void Lock()
    {
        rtos::port::DisableInterruptMasking();

        void* owner = lock;

        while (owner != nullptr)
        {
            blockedList.AddBack(currentTaskControlBlock->blockedItem);

            rtos::port::RestoreInterruptMasking();

            //blockTask();

            rtos::port::DisableInterruptMasking();
            owner = lock;
        }

        lock = currentTaskControlBlock;

        rtos::port::RestoreInterruptMasking();
    }

    void Unlock()
    {
        rtos::port::DisableInterruptMasking();

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

        rtos::port::RestoreInterruptMasking();
    }

    void* lock = nullptr;
    List<Task> blockedList;
};