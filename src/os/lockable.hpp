#pragma once

#include "infra/List.hpp"
#include "scheduler.hpp"
#include "utils.hpp"

#include <atomic>

struct Lockable
{
    void Lock()
    {
        DisableInterrupts();

        void* owner = lock;

        while (owner != nullptr)
        {
            blockedList.AddBack(currentTaskControlBlock->blockedItem);

            EnableInterrupts();

            //blockTask();

            DisableInterrupts();
            owner = lock;
        }

        lock = currentTaskControlBlock;

        EnableInterrupts();
    }

    void Unlock()
    {
        DisableInterrupts();

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

        EnableInterrupts();
    }

    void* lock = nullptr;
    List<Task> blockedList;
};