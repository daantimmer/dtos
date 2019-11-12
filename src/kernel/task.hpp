
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

struct Task
{
    Task(void (*entry)(), uint32_t* stackTop, size_t stackSize, TaskDebugGpio gpioDebug = {});

    Task(const Task&) = delete;
    Task(Task&&) = delete;

    Task& operator=(const Task&) = delete;
    Task& operator=(Task&&) = delete;

    bool StackSafe() const;

    template<uint32_t SIZE>
    struct WithStack;

    void* GetStackPointer() const;
    void SetStackPointer(void* const stackPointer);

protected:
    uint32_t* stackPointer; /* Always as the first element. */
    // void* stackPointer;     /* Always as the first element. */

    uint32_t* const stackTop;
    const size_t stackSize;

public:
    List<Task>::Item queueItem;
    List<Task>::Item blockedItem;

    uint32_t* stackGuard_begin;
    uint32_t* stackGuard_end;

    uint32_t tickDelay = 0;
    uint32_t priority = UINT32_MAX;

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