#ifndef KERNEL_TASKCONTROLBLOCK_HPP
#define KERNEL_TASKCONTROLBLOCK_HPP

#include "infra/util/Function.hpp"
#include "infra/util/IntrusiveList.hpp"
#include "kernel/stack.hpp"
#include "kernel/unblockReason.hpp"
#include <cstdint>

namespace kernel
{
    enum class TaskState
    {
        Created,
        Ready,
        Running,
        Blocked,
        Suspended,
    };

    struct RunnableTask;
    struct TaskControlBlock;

    using UnblockFunction = infra::Function<void(TaskControlBlock&, UnblockReason)>;

    struct TaskControlBlock: infra::IntrusiveList<TaskControlBlock>::NodeType
    {
        TaskControlBlock(Stack&& stack, const char* name, std::uint32_t priority, RunnableTask& owner);

        Stack& GetStack();
        const Stack& GetStack() const;

        const char* Name() const;

        std::uint32_t EffectivePriority() const;
        std::uint32_t Priority() const;
        void Priority(std::uint32_t);

        TaskState State() const;
        void State(TaskState);

        RunnableTask& Owner();
        const RunnableTask& Owner() const;

        void BlockHook(const UnblockFunction& func);
        void UnblockHook(UnblockReason reason);

        infra::IntrusiveList<TaskControlBlock>* ParentList();
        void ParentList(infra::IntrusiveList<TaskControlBlock>*);

    private:
        Stack stack;

        const char* name;

        std::uint32_t priority;
        std::uint32_t minimumPriority;

        TaskState state;

        RunnableTask& owner;
        infra::IntrusiveList<TaskControlBlock>* parentList{nullptr};

        UnblockFunction unblockHook;
    };
}
#endif