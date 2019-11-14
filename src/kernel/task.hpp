
#pragma once

#include "infra/List.hpp"

#include <array>
#include <cstdint>
#include <cstdlib>

struct Task;

struct TaskDebugGpio
{
    void* port = nullptr;
    std::uint32_t pin = 0;
};

struct RunnableTask
{
    RunnableTask()
        : queueItem(this)
        , blockedItem(this)
    {
    }

    virtual void Run() = 0;
    virtual void* GetStackPointer() const = 0;
    virtual void SetStackPointer(void* const stackPointer) = 0;

    List<RunnableTask>::Item queueItem;
    List<RunnableTask>::Item blockedItem;

    uint32_t tickDelay = 0;
    uint32_t priority = UINT32_MAX;
};

struct TaskStack : RunnableTask
{
    virtual void* GetStackPointer() const final;
    virtual void SetStackPointer(void* const stackPointer) final;

    TaskStack(std::uint32_t* stack);

protected:
    std::uint32_t* stackPointer; /* Always as the first element. */
};

struct Task : TaskStack
{
    Task(void (*entry)(), uint32_t* stackTop, size_t stackSize, TaskDebugGpio gpioDebug = {});

    Task(const Task&) = delete;
    Task(Task&&) = delete;

    Task& operator=(const Task&) = delete;
    Task& operator=(Task&&) = delete;

    bool StackSafe() const;

    template<uint32_t SIZE>
    struct WithStack;

    virtual void Run() final;

protected:
    uint32_t* const stackTop;
    const size_t stackSize;

    void (*entry)();

public:
    uint32_t* stackGuard_begin;
    uint32_t* stackGuard_end;

    const TaskDebugGpio gpioDebug = {};
};

template<uint32_t SIZE>
struct Task::WithStack : Task
{
    WithStack(void (*entry)(), TaskDebugGpio gpioDebug = {})
        : Task(entry, stack.data(), stack.size(), std::move(gpioDebug))
    {
    }

protected:
    std::array<uint32_t, SIZE> stack;
};