#pragma once

#include "infra/util/IntrusivePriorityQueue.hpp"
#include "kernel/task.hpp"

namespace kernel
{
struct MainThread;

using RunnableTask = ::RunnableTask;

struct PriorityCompare
{
    bool operator()(const kernel::RunnableTask& lhs, const kernel::RunnableTask& rhs) const
    {
        return lhs.priority < rhs.priority;
    }
};

struct Kernel
{
    Kernel(MainThread&);

    infra::IntrusivePriorityQueue<kernel::RunnableTask, PriorityCompare> priorityQueue;
};

Kernel& GetKernel();
}