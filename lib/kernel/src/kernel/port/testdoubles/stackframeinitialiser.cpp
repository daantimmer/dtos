#include "kernel/port/stackframeinitialiser.hpp"
#include "kernel/task.hpp"

static void ThreadRunner(kernel::RunnableTask* runnable)
{
    runnable->Run();
}

std::uint32_t* kernel::port::InitialiseStack(void*, std::uint32_t*, StackSize_t)
{
    return nullptr;
}
