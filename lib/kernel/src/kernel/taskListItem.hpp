#ifndef KERNEL_TASKLISTITEM_HPP
#define KERNEL_TASKLISTITEM_HPP

#include "infra/util/IntrusiveList.hpp"

namespace kernel
{
    struct RunnableTask;

    struct TaskListItem: infra::IntrusiveList<TaskListItem>::NodeType
    {
        TaskListItem(RunnableTask* task)
            : task{task}
        {}

        friend bool operator>(const TaskListItem& lhs, const TaskListItem& rhs);
        friend bool operator<(const TaskListItem& lhs, const TaskListItem& rhs);

        RunnableTask* task;
    };

    bool operator>(const TaskListItem& lhs, const TaskListItem& rhs);

    bool operator<(const TaskListItem& lhs, const TaskListItem& rhs);
}

#endif
