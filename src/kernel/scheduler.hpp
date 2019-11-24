
#pragma once

#include <chrono>

struct RunnableTask;

extern "C"
{
    extern RunnableTask* volatile currentTaskControlBlock;

    void TaskScheduler();
}

void TriggerTaskSwitch();
bool SchedulerTick();

void YieldTask();
void BlockTask();

void DelayTask(std::chrono::microseconds delay);
void DelayTask(std::chrono::milliseconds delay);
void DelayTask(uint32_t ticks = 0);
void DelayTask(RunnableTask& task, uint32_t ticks = 0);