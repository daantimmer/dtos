
#include "task.hpp"
#include "kernel/kernel.hpp"
#include "kernel/port/stackframeinitialiser.hpp"
#include "kernel/scheduler.hpp"
#include <algorithm>
#include <cassert>

// static void TaskExitError()
// {
//     while (1)
//     {
//         assert(false);
//     }
// }

void RunnableTask::PrepareDelay(std::uint32_t interval)
{
    this->interval = interval;
    tickDelay = static_cast<std::uint32_t>(kernel::GetKernel().systicks) + this->interval;
}

void RunnableTask::Delay()
{
    kernel::GetKernel().delayedTasks.insert(*this);
    TriggerTaskSwitch();
}

TaskStack::TaskStack(std::uint32_t* stack)
    : stackPointer(stack)
{}

auto TaskStack::GetStackPointer() const -> void*
{
    return stackPointer;
}

void TaskStack::SetStackPointer(void* const stackPointer)
{
    this->stackPointer = reinterpret_cast<std::uint32_t*>(stackPointer);
}

Task::Task(void (*entry)(Task& task), uint32_t* stackTop, kernel::StackSize_t stackSize, TaskDebugGpio gpioDebug)
    : TaskStack{stackTop + 1}
    , stackTop(stackTop + 1)
    , stackSize(kernel::StackSize_t{static_cast<std::size_t>(stackSize) - 2})
    , entry(entry)
    // , queueItem(this)
    // , blockedItem(this)
    , stackGuard_begin(this->stackTop - 1)
    , stackGuard_end(stackGuard_begin + static_cast<std::size_t>(stackSize))
    , gpioDebug(gpioDebug)
{
    std::fill(stackGuard_begin, stackGuard_end, 0xDEADBEEF);
    *stackGuard_begin = 0xCCCCCCCC;
    *stackGuard_end = 0xCDCDCDCD;

    std::tie(std::ignore, stackPointer) = kernel::port::InitialiseStack(this, this->stackTop, this->stackSize);
}

void Task::Run()
{
    entry(*this);
}

auto Task::StackSafe() const -> bool
{
    if (stackGuard_begin == nullptr || stackGuard_end == nullptr)
    {
        return true;
    }

    return (*stackGuard_begin == 0xCCCCCCCC) && (*stackGuard_end == 0xCDCDCDCD);
}
