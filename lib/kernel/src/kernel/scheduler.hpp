#ifndef KERNEL_SCHEDULER_HPP
#define KERNEL_SCHEDULER_HPP

#include "kernel/status.hpp"
#include "kernel/systicks_t.hpp"
#include "kernel/task.hpp"
#include "kernel/taskList.hpp"
#include <chrono>
#include <cstdint>

namespace kernel
{
    struct RunnableTask;
}

extern "C"
{
    extern kernel::RunnableTask* volatile currentTaskControlBlock;

    void TaskScheduler();
}

auto SchedulerTick() -> bool;

void YieldTask();
// void BlockTask();

void DelayTask(std::chrono::microseconds delay);
void DelayTask(std::chrono::milliseconds delay);
void DelayTask(std::uint32_t ticks = 0);
void DelayTask(kernel::RunnableTask& task, std::uint32_t ticks = 0);

namespace kernel
{
    struct MainThread;

    struct Scheduler
    {
        Scheduler(MainThread&);
        ~Scheduler();

        Scheduler(const Scheduler&) = delete;
        Scheduler(Scheduler&&) = delete;

        Scheduler& operator=(const Scheduler&) = delete;
        Scheduler& operator=(Scheduler&&) = delete;

        bool Tick();

        StatusCode Block(TaskList<>& blockList, const kernel::UnblockFunction& = {});
        StatusCode Block(TaskList<>& blockList, RunnableTask& task, const kernel::UnblockFunction& = {});

        void Unblock(RunnableTask& task);

        static auto CurrentTask() -> RunnableTask&;
        auto GetIdleTask() const -> RunnableTask&;

        SysTicks_t systicks{std::uint32_t{0u}};

        TaskList<> delayedTasksV2;
        TaskList<> readyTasksV2;

        mutable StaticTask<64> idleTask;

    private:
        StatusCode InternalBlock(TaskList<>& blockList, RunnableTask& task, UnblockFunction unblockFunction);
        void InternalUnblock(RunnableTask& task, UnblockReason unblockReason);
    };
}

#endif
