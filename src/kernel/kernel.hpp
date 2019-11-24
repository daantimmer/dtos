#pragma once

#include "elib/intrusivelist.hpp"
#include "elib/sortedintrusivelist.hpp"
#include "kernel/task.hpp"

namespace kernel
{
struct MainThread;

using RunnableTask = ::RunnableTask;

struct RunnableCompare
{
    bool operator()(const kernel::RunnableTask& lhs, const kernel::RunnableTask& rhs) const
    {
        return lhs.priority > rhs.priority;
    }
};

struct Kernel
{
    Kernel(MainThread&);

    RunnableTask& CurrentTask() const;

    RunnableTask& GetIdleTask() const;

    std::uint32_t systicks = 0;

    elib::
    SortedIntrusiveList<RunnableCompare, kernel::RunnableTask, &kernel::RunnableTask::queueItem, kernel::RunnableTask>
    delayedTasks;
    elib::
    SortedIntrusiveList<RunnableCompare, kernel::RunnableTask, &kernel::RunnableTask::queueItem, kernel::RunnableTask>
    readyTasks;
    elib::
    SortedIntrusiveList<RunnableCompare, kernel::RunnableTask, &kernel::RunnableTask::queueItem, kernel::RunnableTask>
    blockedTasks;

    mutable Task::WithStack<32> idleTask;
};
}