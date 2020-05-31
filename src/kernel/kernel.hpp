#pragma once

#include "elib/intrusivelist.hpp"
#include "elib/sortedintrusivelist.hpp"
#include "kernel/systicks_t.hpp"
#include "kernel/task.hpp"

namespace kernel
{
    struct MainThread;

    using RunnableTask = ::RunnableTask;

    struct RunnableCompare
    {
        auto operator()(const kernel::RunnableTask& lhs, const kernel::RunnableTask& rhs) const -> bool
        {
            return lhs.priority > rhs.priority;
        }
    };

    using RunnableTaskList = elib::SortedIntrusiveList<RunnableCompare,
                                                       kernel::RunnableTask,
                                                       &kernel::RunnableTask::queueItem,
                                                       kernel::RunnableTask>;

    struct Kernel
    {
        Kernel(MainThread&);

        static auto CurrentTask() -> RunnableTask&;
        auto GetIdleTask() const -> RunnableTask&;

        SysTicks_t systicks{std::uint32_t{0u}};

        RunnableTaskList delayedTasks;
        RunnableTaskList readyTasks;
        RunnableTaskList blockedTasks;

        mutable Task::WithStack<32> idleTask;
    };
}