#ifndef KERNEL_TASKLIST_HPP
#define KERNEL_TASKLIST_HPP

#include "infra/util/ext/SortedIntrusiveList.hpp"
#include "kernel/taskListItem.hpp"

namespace kernel
{
    template <class TCompare = std::greater<TaskListItem>>
    using TaskList = infra::SortedIntrusiveList<TaskListItem, TCompare>;
}

#endif
