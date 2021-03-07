#include "kernel/taskListItem.hpp"
#include "kernel/task.hpp"

bool kernel::operator>(const TaskListItem& lhs, const TaskListItem& rhs)
{
    return lhs.task->priority > rhs.task->priority;
}

bool kernel::operator<(const TaskListItem& lhs, const TaskListItem& rhs)
{
    /* TODO have to fix */
    return lhs.task->priority < rhs.task->priority;
}