#ifndef KERNEL_TASK_HPP
#define KERNEL_TASK_HPP

#include "elib/intrusivelist.hpp"
#include "infra/List.hpp"
// #include "infra/util/IntrusiveList.hpp"
// #include "infra/util/IntrusivePriorityQueue.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/ext/SortedIntrusiveList.hpp"
#include "kernel/port/systemtick.hpp"
#include "kernel/stacksize.hpp"
#include "kernel/taskListItem.hpp"
#include "kernel/unblockReason.hpp"
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <functional>

namespace kernel
{
    struct Task;
    struct RunnableTask;

    struct TaskDebugGpio
    {
        void* port = nullptr;
        std::uint32_t pin = 0;
    };

    using UnblockFunction = infra::Function<void(RunnableTask&, UnblockReason)>;

    struct RunnableTask
    {
        RunnableTask() = default;

        RunnableTask(const RunnableTask&) = delete;
        RunnableTask(RunnableTask&&) = delete;

        RunnableTask& operator=(const RunnableTask&) = delete;
        RunnableTask& operator=(RunnableTask&&) = delete;

        virtual void Run() = 0;
        virtual void* GetStackPointer() const = 0;
        virtual void SetStackPointer(void* const stackPointer) = 0;
        virtual bool StackSafe() const = 0;
        virtual std::size_t StackAvailable() = 0;

        TaskListItem queueItemV2{this};
        // TaskListItem blockedItemV2{this};

        // elib::IntrusiveListNode queueItem{};
        elib::IntrusiveListNode blockedItem{};

        void* blockedBy = nullptr;

        template <class Function>
        void RepeatEvery(std::chrono::milliseconds interval, Function function)
        {
            PrepareDelay(std::chrono::duration_cast<systemtick::ticks>(interval).count());

            while (true)
            {
                function();

                Delay();
            }
        }

        void BlockHook(UnblockFunction func);
        void UnblockHook(UnblockReason reason);

        std::uint32_t tickDelay = 0;
        std::uint32_t interval = 0;

        std::uint32_t priority = UINT32_MAX;

        const char* name = "unknown";

    protected:
        void PrepareDelay(std::uint32_t interval);
        void Delay();

        UnblockFunction unblockHook;
    };

    struct TaskStack: RunnableTask
    {
        auto GetStackPointer() const -> void* final;
        void SetStackPointer(void* stackPointer) final;

        TaskStack(std::uint32_t* stack);

    protected:
        std::uint32_t* stackPointer;
    };

    struct Task: TaskStack
    {
        Task(void (*entry)(const Task& task, void*),
             uint32_t* stackTop,
             kernel::StackSize_t stackSize,
             void* param = nullptr/*,
             TaskDebugGpio gpioDebug = {}*/);

        Task(const Task&) = delete;
        Task(Task&&) = delete;

        auto operator=(const Task&) & -> Task& = delete;
        auto operator=(Task&&) && -> Task& = delete;

        bool StackSafe() const override;
        std::size_t StackAvailable() override;

        template <std::size_t SIZE>
        struct WithStack;

        void Run() final;

    protected:
        std::uint32_t* const stackTop;
        const kernel::StackSize_t stackSize;

        void (*entry)(const Task& task, void*);

        void* param;

        std::uint32_t* stackEdge;

    public:
        std::uint32_t* stackGuard_begin;
        std::uint32_t* stackGuard_end;

        /*const TaskDebugGpio gpioDebug = {};*/
    };

    template <std::size_t SIZE>
    struct Task::WithStack: Task
    {
        WithStack(void (*entry)(const Task& task, void*), void* param = nullptr /*, TaskDebugGpio gpioDebug = {}*/)
            : Task(entry,
                   stack.data(), // NOLINT: stack is initialized by Task
                   kernel::StackSize_t{SIZE},
                   param/*,
                   std::move(gpioDebug)*/)
        {}

        bool StackSafe() const final
        {
            if (stackGuard_begin == nullptr || stackGuard_end == nullptr)
            {
                return true;
            }

            if ((*stackGuard_begin == 0xCCCCCCCC) && (*stackGuard_end == 0xCDCDCDCD))
            {
                return true;
            }

            return false;
        }

    protected:
        std::array<uint32_t, SIZE> stack;
    };
}

#endif
