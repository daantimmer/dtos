#ifndef KERNEL_TASKLIST_HPP
#define KERNEL_TASKLIST_HPP

#include "infra/util/ext/SortedIntrusiveList.hpp"
#include "kernel/task.hpp"

namespace kernel
{
    template <class TCompare = std::greater<TaskControlBlock>>
    using TaskList = infra::SortedIntrusiveList<TaskControlBlock, TCompare>;

    bool operator>(const TaskControlBlock& lhs, const TaskControlBlock& rhs);
}

#endif
