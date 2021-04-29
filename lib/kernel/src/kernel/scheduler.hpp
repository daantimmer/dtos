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
    struct TaskControlBlock;
}

extern "C"
{
    extern kernel::TaskControlBlock* volatile currentTaskControlBlock;

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
        StatusCode Add(TaskControlBlock& ctrlBlock);

        StatusCode Block(TaskList<>& blockList, const kernel::UnblockFunction& = {});
        StatusCode Block(TaskList<>& blockList, TaskControlBlock& ctrlBlock, const kernel::UnblockFunction& = {});

        void Unblock(TaskControlBlock& ctrlBlock);

        static auto CurrentTask() -> RunnableTask&;
        TaskControlBlock& GetIdleTask() const;

        SysTicks_t systicks{std::uint32_t{0u}};

        TaskList<> delayedTasksV2;
        TaskList<> readyTasksV2;

        mutable StaticTask<64> idleTask;

    private:
        StatusCode InternalBlock(TaskList<>& blockList, TaskControlBlock& ctrlBlock, UnblockFunction unblockFunction);
        void InternalUnblock(TaskControlBlock& ctrlBlock, UnblockReason unblockReason);
    };
}

#endif
