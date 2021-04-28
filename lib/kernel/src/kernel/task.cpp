
#include "task.hpp"
#include "kernel/getkernel.hpp"
#include "kernel/port/stackframeinitialiser.hpp"
#include "kernel/port/triggerTaskSwitch.hpp"
#include "kernel/scheduler.hpp"
#include <algorithm>
#include <cassert>

namespace kernel
{
    /* TaskControlBlock */
    TaskControlBlock::TaskControlBlock(Stack&& stack,
                                       const char* name,
                                       const std::uint32_t priority,
                                       RunnableTask& owner)
        : stack{std::move(stack)}
        , priority{priority}
        , state{TaskState::Created}
        , owner{owner}
    {}

    Stack& TaskControlBlock::GetStack()
    {
        return stack;
    }

    const Stack& TaskControlBlock::GetStack() const
    {
        return stack;
    }

    const char* TaskControlBlock::Name() const
    {
        return name;
    }

    std::uint32_t TaskControlBlock::EffectivePriority() const
    {
        return minimumPriority > priority ? minimumPriority : priority;
    }

    std::uint32_t TaskControlBlock::Priority() const
    {
        return minimumPriority;
    }

    void TaskControlBlock::Priority(const std::uint32_t priority)
    {
        this->minimumPriority = priority;
    }

    TaskState TaskControlBlock::State() const
    {
        return state;
    }

    void TaskControlBlock::State(const TaskState state)
    {
        this->state = state;
    }

    RunnableTask& TaskControlBlock::Owner()
    {
        return owner;
    }

    const RunnableTask& TaskControlBlock::Owner() const
    {
        return owner;
    }

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
}

/* TODO: To be moved to task control block structure */
void kernel::RunnableTask::BlockHook(UnblockFunction func)
{
    unblockHook = func;
}

void kernel::RunnableTask::UnblockHook(UnblockReason reason)
{
    auto unblockFunc = unblockHook;
    unblockHook = {};

    if (static_cast<bool>(unblockFunc) == true)
    {
        unblockFunc(*this, reason);
    }
}

void kernel::RunnableTask::PrepareDelay(std::uint32_t interval)
{
    this->interval = interval;
    tickDelay = static_cast<std::uint32_t>(kernel::GetScheduler().systicks) + this->interval;
}

void kernel::RunnableTask::Delay()
{
    // kernel::GetScheduler().delayedTasks.insert(*this);

    kernel::GetScheduler().delayedTasksV2.push(static_cast<TaskBase*>(this)->GetTaskControlBlock());

    TriggerTaskSwitch();
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