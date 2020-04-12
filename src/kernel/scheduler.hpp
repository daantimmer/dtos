
#pragma once

#include <chrono>
#include <cstdint>

struct RunnableTask;

extern "C"
{
    extern RunnableTask* volatile currentTaskControlBlock;

    void TaskScheduler();
}

void TriggerTaskSwitch();
auto SchedulerTick() -> bool;

void YieldTask();
void BlockTask();

void DelayTask(std::chrono::microseconds delay);
void DelayTask(std::chrono::milliseconds delay);
void DelayTask(std::uint32_t ticks = 0);
void DelayTask(RunnableTask& task, std::uint32_t ticks = 0);