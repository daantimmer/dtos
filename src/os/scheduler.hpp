
#pragma once

#include "task.hpp"
#include <chrono>

extern "C"
{
    extern Task* volatile currentTaskControlBlock;
    
    void TaskScheduler();
}

bool SchedulerTick();
void startFirstTask() __attribute__((naked));

void YieldTask();
void BlockTask();

void DelayTask(std::chrono::microseconds delay);
void DelayTask(std::chrono::milliseconds delay);
void DelayTask(uint32_t ticks = 0);
void DelayTask(Task& task, uint32_t ticks = 0);