// #pragma once

// #include "elib/intrusivelist.hpp"
// #include "infra/List.hpp"
// #include "kernel/InterruptMasking.hpp"
// #include "scheduler.hpp"

// #include <atomic>

// struct Lockable
// {
//     void Lock()
//     {
//         kernel::port::DisableInterruptMasking();

//         void* owner = lock;

//         while (owner != nullptr)
//         {
//             blockedList.push_back(*currentTaskControlBlock);

//             kernel::port::RestoreInterruptMasking();

//             //blockTask();

//             kernel::port::DisableInterruptMasking();
//             owner = lock;
//         }

//         lock = currentTaskControlBlock;

//         kernel::port::RestoreInterruptMasking();
//     }

//     void Unlock()
//     {
//         kernel::port::DisableInterruptMasking();

//         if (lock == currentTaskControlBlock)
//         {
//             lock = nullptr;

//             while (blockedList.empty() == false)
//             {
//                 //auto task = blockedList.PeekFront();
//                 blockedList.pop_front();

//                 //unblockTask(task);
//             }
//         }

//         kernel::port::RestoreInterruptMasking();
//     }

//     void* lock = nullptr;
//     elib::IntrusiveList<RunnableTask, &RunnableTask::blockedItem, RunnableTask> blockedList;
// };