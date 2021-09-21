#include "taskControlBlock.hpp"
#include "kernel/getkernel.hpp"
#include "kernel/port/stackframeinitialiser.hpp"
#include "kernel/port/triggerTaskSwitch.hpp"
#include "kernel/scheduler.hpp"
#include "task.hpp"
#include <algorithm>
#include <cassert>

namespace kernel
{
    /* TaskControlBlock */
    TaskControlBlock::TaskControlBlock(Stack&& stack,
                                       const char* name,
                                       const std::uint32_t priority,
                                       RunnableTask& owner)
        : stack{stack}
        , name{name}
        , priority{priority}
        , minimumPriority{priority}
        , state{TaskState::Created}
        , owner{owner}
    {}

    Stack& TaskControlBlock::GetStack()
    {
        return stack;
    }

    const Stack& TaskControlBlock::GetStack() const
    {
        return stack;
    }

    const char* TaskControlBlock::Name() const
    {
        return name;
    }

    std::uint32_t TaskControlBlock::EffectivePriority() const
    {
        return minimumPriority > priority ? minimumPriority : priority;
    }

    std::uint32_t TaskControlBlock::Priority() const
    {
        return minimumPriority;
    }

    void TaskControlBlock::Priority(const std::uint32_t priority)
    {
        this->minimumPriority = priority;
    }

    TaskState TaskControlBlock::State() const
    {
        return state;
    }

    void TaskControlBlock::State(const TaskState state)
    {
        this->state = state;
    }

    RunnableTask& TaskControlBlock::Owner()
    {
        return owner;
    }

    const RunnableTask& TaskControlBlock::Owner() const
    {
        return owner;
    }

    void TaskControlBlock::BlockHook(const UnblockFunction& func)
    {
        unblockHook = func;
    }

    void TaskControlBlock::UnblockHook(UnblockReason reason)
    {
        auto unblockFunc = unblockHook;
        unblockHook = {};

        if (static_cast<bool>(unblockFunc) == true)
        {
            unblockFunc(*this, reason);
        }
    }

    infra::IntrusiveList<TaskControlBlock>* TaskControlBlock::ParentList()
    {
        return parentList;
    }

    void TaskControlBlock::ParentList(infra::IntrusiveList<TaskControlBlock>* parentList)
    {
        this->parentList = parentList;
    }
}
