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

auto SchedulerTick() -> bool;

void YieldTask();
void DelayTask(std::chrono::microseconds delay);
void DelayTask(std::chrono::milliseconds delay);
void DelayTask(std::uint32_t ticks = 0);
void DelayTask(kernel::TaskControlBlock& ctrlBlock, std::uint32_t ticks = 0);

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
        void* SwitchContext(void*);

        StatusCode Add(TaskControlBlock& ctrlBlock);

        StatusCode Yield();
        StatusCode Yield(TaskControlBlock& ctrlBlock);

        StatusCode Block(TaskList<>& blockList, const kernel::UnblockFunction& = {});
        StatusCode Block(TaskList<>& blockList, TaskControlBlock& ctrlBlock, const kernel::UnblockFunction& = {});

        void Unblock(TaskControlBlock& ctrlBlock);

        static auto CurrentTask() -> RunnableTask&;
        TaskControlBlock& GetIdleTask() const;

        TaskControlBlock& CurrentTaskControlBlock()
        {
            return *currentTaskControlBlock;
        }

        SysTicks_t systicks{std::uint32_t{0u}};

        TaskList<> delayedTasksV2;

        mutable StaticTask<64> idleTask;

    private:
        void InternalUnblock(TaskControlBlock& ctrlBlock, UnblockReason unblockReason);

        TaskList<> readyTasksV2;

        TaskControlBlock* volatile currentTaskControlBlock;
    };
}

#endif
