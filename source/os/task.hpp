
#pragma once

#include <cstdint>
#include <cstdlib> 

#include "infra/DoubleLinkedList.hpp"

struct Task;

struct TaskDebugGpio
{
    void* port;
    std::uint32_t pin;
};

struct Task
{
    Task(void (*entry)(), uint32_t* stackTop, size_t stackSize, TaskDebugGpio gpioDebug = {nullptr, 0});

protected:
    uint32_t* stackPointer;
    uint32_t* stackTop;
    size_t stackSize;

public:
    DoubleLinkedList<Task>::Item* llItem = nullptr;
    TaskDebugGpio gpioDebug = {nullptr, 0};
};