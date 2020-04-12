
#pragma once

#include "elib/intrusivelist.hpp"
#include "infra/List.hpp"
#include "kernel/getkernel.hpp"
#include "kernel/port/systemtick.hpp"
#include "kernel/stacksize.hpp"
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>

struct Task;

struct TaskDebugGpio
{
    void* port = nullptr;
    std::uint32_t pin = 0;
};

struct TaskListItem
{};

struct RunnableTask: TaskListItem
{
    RunnableTask()
    {}

    virtual void Run() = 0;
    virtual void* GetStackPointer() const = 0;
    virtual void SetStackPointer(void* const stackPointer) = 0;

    elib::IntrusiveListNode queueItem{};
    elib::IntrusiveListNode blockedItem{};
    // List<RunnableTask>::Item queueItem;
    // List<RunnableTask>::Item blockedItem;

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

    std::uint32_t tickDelay = 0;
    std::uint32_t interval = 0;

    std::uint32_t priority = UINT32_MAX;

protected:
    void PrepareDelay(std::uint32_t interval);
    void Delay();
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
    Task(void (*entry)(Task& task), uint32_t* stackTop, kernel::StackSize_t stackSize, TaskDebugGpio gpioDebug = {});

    Task(const Task&) = delete;
    Task(Task&&) = delete;

    auto operator=(const Task&) & -> Task& = delete;
    auto operator=(Task&&) && -> Task& = delete;

    auto StackSafe() const -> bool;

    template <std::size_t SIZE>
    struct WithStack;

    void Run() final;

protected:
    std::uint32_t* const stackTop;
    const kernel::StackSize_t stackSize;

    void (*entry)(Task& task);

public:
    std::uint32_t* stackGuard_begin;
    std::uint32_t* stackGuard_end;

    const TaskDebugGpio gpioDebug = {};
};

template <std::size_t SIZE>
struct Task::WithStack: Task
{
    WithStack(void (*entry)(Task& task), TaskDebugGpio gpioDebug = {})
        : Task(entry, stack.data(), kernel::StackSize_t{SIZE}, std::move(gpioDebug))
    {}

protected:
    std::array<uint32_t, SIZE> stack;
};