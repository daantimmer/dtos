#ifndef KERNEL_TASK_HPP
#define KERNEL_TASK_HPP

#include "infra/util/Function.hpp"
#include "infra/util/IntrusiveList.hpp"
#include "kernel/port/stackframeinitialiser.hpp"
#include "kernel/port/systemtick.hpp"
#include "kernel/stack.hpp"
#include "kernel/stacksize.hpp"
#include "kernel/unblockReason.hpp"
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>

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

    using UnblockFunction = infra::Function<void(RunnableTask&, UnblockReason)>;

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

    private:
        Stack stack;

        const char* name;

        std::uint32_t priority;
        std::uint32_t minimumPriority;

        TaskState state;

        RunnableTask& owner;

        UnblockFunction unblockHook;
    };

    struct Task
    {
        virtual ~Task() = default;

        virtual std::uint32_t EffectivePriority() const = 0;
        virtual std::uint32_t Priority() const = 0;
        virtual void Priority(std::uint32_t) = 0;

        virtual TaskState State() const = 0;
        virtual void State(TaskState) = 0;
    };

    struct RunnableTask: Task
    {
        virtual ~RunnableTask() = default;

        virtual void Run() = 0;

        /* TODO: To be moved to task control block structure */
        void BlockHook(UnblockFunction func);
        void UnblockHook(UnblockReason reason);

        std::uint32_t tickDelay = 0;
        std::uint32_t interval = 0;

        std::uint32_t priority = UINT32_MAX;

    protected:
        UnblockFunction unblockHook;
    };

    struct TaskBase: RunnableTask
    {
        TaskBase(Stack&& stack, const char* name)
            : taskControlBlock{std::move(stack), name, 0, *this}
        {}

        /* RunnableTask::Task */
        std::uint32_t EffectivePriority() const final;

        std::uint32_t Priority() const final;

        void Priority(std::uint32_t) final;

        TaskState State() const final;

        void State(TaskState) final;

        TaskControlBlock& GetTaskControlBlock();

        const TaskControlBlock& GetTaskControlBlock() const;

        void Start()
        {
            GetTaskControlBlock().GetStack().Initialize(this);
        }

    private:
        TaskControlBlock taskControlBlock;
    };

    template <std::size_t TStackSize>
    struct StaticTask: TaskBase
    {
        StaticTask(const char* name, void (*entry)(const RunnableTask& task, void*), void* param = nullptr)
            : TaskBase{{staticStack.stack, TStackSize}, name}
            , entry{entry}
            , param{param}
        {
            Start();
        }

        /* TaskBase::TaskRunner::Run */
        void Run() final
        {
            entry(*this, param);
        }

    private:
        StaticStack<TStackSize> staticStack;
        void (*entry)(const RunnableTask& task, void*);
        void* param;
    };

    //
    //
    //
    //
    //
    //
    //

    // template <std::size_t SIZE>
    // struct TaskStack: RunnableTask
    // {
    //     auto GetStackPointer() const -> void* final;
    //     void SetStackPointer(void* stackPointer) final;

    //     TaskStack(std::uint32_t* stack, std::size_t stackSize);

    // protected:
    //     // std::uint32_t* stackPointer;
    //     // kernel::Stack<SIZE> stack;
    // };

    // template <std::size_t SIZE>
    // struct Task: TaskStack<SIZE>
    // {
    //     Task(void (*entry)(const Task& task, void*),
    //         //  uint32_t* stackTop,
    //         //  kernel::StackSize_t stackSize,
    //          void* param = nullptr/*,
    //          TaskDebugGpio gpioDebug = {}*/);

    //     Task(const Task&) = delete;
    //     Task(Task&&) = delete;

    //     auto operator=(const Task&) & -> Task& = delete;
    //     auto operator=(Task&&) && -> Task& = delete;

    //     bool StackSafe() const override;
    //     std::size_t StackAvailable() override;

    //     template <std::size_t TSIZE>
    //     struct WithStack;

    //     void Run() final;

    // protected:
    //     // std::uint32_t* const stackTop;
    //     const kernel::StackSize_t stackSize;

    //     void (*entry)(const Task& task, void*);

    //     void* param;

    //     // std::uint32_t* stackEdge;

    // public:
    //     // std::uint32_t* const stackGuard_begin;
    //     // std::uint32_t* const stackGuard_end;
    // };

    // template <std::size_t SIZE>
    // TaskStack<SIZE>::TaskStack(std::uint32_t*, std::size_t)
    // // : stackPointer(stack)
    // // , stack{stack, stackSize}
    // // : stack{stack, stackSize}
    // {}

    // template <std::size_t SIZE>
    // auto TaskStack<SIZE>::GetStackPointer() const -> void*
    // {
    //     return stack.GetStackPointer();
    //     // return stackPointer;
    // }

    // template <std::size_t SIZE>
    // void TaskStack<SIZE>::SetStackPointer(void* const stackPointer)
    // {
    //     stack.SetStackPointer(reinterpret_cast<std::uint32_t*>(stackPointer));
    //     // this->stackPointer = reinterpret_cast<std::uint32_t*>(stackPointer);
    // }

    // template <std::size_t SIZE>
    // Task<SIZE>::Task(void (*entry)(const Task& task, void*),
    //             //    uint32_t* stackTop,
    //             //    StackSize_t stackSize,
    //                void* param/*,
    //                TaskDebugGpio gpioDebug*/)
    // // : TaskStack{stackTop + 1} // NOLINT
    // : TaskStack<SIZE>{nullptr, 0} // NOLINT
    // // , stackTop(stackTop + 1) // NOLINT
    // , stackSize(kernel::StackSize_t{static_cast<std::size_t>(SIZE) - 2})
    // , entry(entry)
    // , param(param)
    // // , queueItem(this)
    // // , blockedItem(this)
    // // , stackGuard_begin(this->stackTop - 1) // NOLINT
    // // , stackGuard_end(stackGuard_begin + static_cast<std::size_t>(SIZE)-1) // NOLINT
    // /*, gpioDebug(gpioDebug)*/
    // {
    //     // std::fill(stackGuard_begin, stackGuard_end, 0xDEADBEEF);
    //     // *stackGuard_begin = 0xCCCCCCCC;
    //     // *stackGuard_end = 0xCDCDCDCD;

    //     // stackPointer = kernel::port::InitialiseStack(this->stackTop, this->stackSize);
    //     // stackPointer = kernel::port::InitialiseStack(this, this->stackTop, this->stackSize);
    //     kernel::port::SetEntry(this, TaskStack<SIZE>::stack.GetStackPointer());
    //     // stackEdge = stack.GetStackPointer();
    // }

    // template <std::size_t SIZE>
    // void Task<SIZE>::Run()
    // {
    //     entry(*this, param);
    // }

    // template <std::size_t SIZE>
    // auto Task<SIZE>::StackSafe() const -> bool
    // {
    //     return TaskStack<SIZE>::stack.IsSafe();
    //     // if (stackGuard_begin == nullptr || stackGuard_end == nullptr)
    //     // {
    //     //     return true;
    //     // }

    //     // if ((*stackGuard_begin == 0xCCCCCCCC) && (*stackGuard_end == 0xCDCDCDCD))
    //     // {
    //     //     return true;
    //     // }

    //     // return false;
    // }

    // template <std::size_t SIZE>
    // std::size_t Task<SIZE>::StackAvailable()
    // {
    //     return 1;
    //     // auto* walker = stackGuard_begin + 1;

    //     // while (walker != stackGuard_end && *walker == 0xDEADBEEF)
    //     // {
    //     //     stackEdge = walker;
    //     //     ++walker;
    //     // }
    //     // return stackEdge - stackGuard_begin;
    // }

    // template <std::size_t SIZE>
    // template <std::size_t TSIZE>
    // struct Task<SIZE>::WithStack: Task<SIZE>
    // {
    //     WithStack(const char* name,
    //               void (*entry)(const Task& task, void*),
    //               void* param = nullptr /*, TaskDebugGpio gpioDebug = {}*/)
    //         : Task(entry,
    //                //    stack.data(), // NOLINT: stack is initialized by Task
    //                //    kernel::StackSize_t{SIZE},
    //                param)
    //     {
    //         this->name = name;
    //     }

    //     // bool StackSafe() const final
    //     // {
    //     //     if (stackGuard_begin == nullptr || stackGuard_end == nullptr)
    //     //     {
    //     //         return true;
    //     //     }

    //     //     if ((*stackGuard_begin == 0xCCCCCCCC) && (*stackGuard_end == 0xCDCDCDCD))
    //     //     {
    //     //         return true;
    //     //     }

    //     //     return false;
    //     // }

    //     // protected:
    //     //     std::array<uint32_t, SIZE> stack;
    // };
}

#endif
