
#include "task.hpp"
#include "kernel/getkernel.hpp"
#include "kernel/port/stackframeinitialiser.hpp"
#include "kernel/port/triggerTaskSwitch.hpp"
#include "kernel/scheduler.hpp"
#include <algorithm>
#include <cassert>

namespace kernel
{
    /* TaskBase */
    std::uint32_t TaskBase::EffectivePriority() const
    {
        return GetTaskControlBlock().Priority();
    }

    std::uint32_t TaskBase::Priority() const
    {
        return GetTaskControlBlock().Priority();
    }

    void TaskBase::Priority(const std::uint32_t priority)
    {
        GetTaskControlBlock().Priority(priority);
    }

    TaskState TaskBase::State() const
    {
        return GetTaskControlBlock().State();
    }

    void TaskBase::State(const TaskState state)
    {
        GetTaskControlBlock().State(state);
    }

    TaskControlBlock& TaskBase::GetTaskControlBlock()
    {
        return taskControlBlock;
    }

    const TaskControlBlock& TaskBase::GetTaskControlBlock() const
    {
        return taskControlBlock;
    }

    void TaskBase::Start()
    {
        GetScheduler().Add(GetTaskControlBlock());
    }
}

// kernel::TaskStack::TaskStack(std::uint32_t* stack, std::size_t stackSize)
// // : stackPointer(stack)
// // , stack{stack, stackSize}
// // : stack{stack, stackSize}
// {}

// auto kernel::TaskStack::GetStackPointer() const -> void*
// {
//     return stack.GetStackPointer();
//     // return stackPointer;
// }

// void kernel::TaskStack::SetStackPointer(void* const stackPointer)
// {
//     stack.SetStackPointer(reinterpret_cast<std::uint32_t*>(stackPointer));
//     // this->stackPointer = reinterpret_cast<std::uint32_t*>(stackPointer);
// }

// kernel::Task::Task(void (*entry)(const Task& task, void*),
//                 //    uint32_t* stackTop,
//                 //    kernel::StackSize_t stackSize,
//                    void* param/*,
//                    TaskDebugGpio gpioDebug*/)
//     // : TaskStack{stackTop + 1} // NOLINT
//     : TaskStack{stackTop, static_cast<std::size_t>(stackSize)} // NOLINT
//     , stackTop(stackTop + 1) // NOLINT
//     , stackSize(kernel::StackSize_t{static_cast<std::size_t>(stackSize) - 2})
//     , entry(entry)
//     , param(param)
//     // , queueItem(this)
//     // , blockedItem(this)
//     , stackGuard_begin(this->stackTop - 1) // NOLINT
//     , stackGuard_end(stackGuard_begin + static_cast<std::size_t>(stackSize)-1) // NOLINT
// /*, gpioDebug(gpioDebug)*/
// {
//     // std::fill(stackGuard_begin, stackGuard_end, 0xDEADBEEF);
//     // *stackGuard_begin = 0xCCCCCCCC;
//     // *stackGuard_end = 0xCDCDCDCD;

//     // stackPointer = kernel::port::InitialiseStack(this->stackTop, this->stackSize);
//     // stackPointer = kernel::port::InitialiseStack(this, this->stackTop, this->stackSize);
//     kernel::port::SetEntry(this, stack.GetStackPointer());
//     stackEdge = stack.GetStackPointer();
// }

// void kernel::Task::Run()
// {
//     entry(*this, param);
// }

// auto kernel::Task::StackSafe() const -> bool
// {
//     return stack.IsSafe();
//     // if (stackGuard_begin == nullptr || stackGuard_end == nullptr)
//     // {
//     //     return true;
//     // }

//     // if ((*stackGuard_begin == 0xCCCCCCCC) && (*stackGuard_end == 0xCDCDCDCD))
//     // {
//     //     return true;
//     // }

//     // return false;
// }

// std::size_t kernel::Task::StackAvailable()
// {
//     auto* walker = stackGuard_begin + 1;

//     while (walker != stackGuard_end && *walker == 0xDEADBEEF)
//     {
//         stackEdge = walker;
//         ++walker;
//     }
//     return stackEdge - stackGuard_begin;
// }