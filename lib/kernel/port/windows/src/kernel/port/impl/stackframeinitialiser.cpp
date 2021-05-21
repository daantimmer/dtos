#include "kernel/port/stackframeinitialiser.hpp"
#include "kernel/task.hpp"

static void ThreadRunner(kernel::RunnableTask* runnable)
{
    runnable->Run();
}

std::uint32_t* kernel::port::InitialiseStack(std::uint32_t* stack, size_t)
{
    return stack;
}

std::uint32_t* kernel::port::InitialiseStack(std::uint32_t* stack, StackSize_t)
{
    return stack;
}

void kernel::port::SetEntry(kernel::RunnableTask*, std::uint32_t*)
{}