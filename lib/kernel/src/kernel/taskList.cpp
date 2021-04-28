
#include "kernel/taskList.hpp"

bool kernel::operator>(const TaskControlBlock& lhs, const TaskControlBlock& rhs)
{
    /* TODO have to fix ?? */
    return lhs.EffectivePriority() > rhs.EffectivePriority();
}
