
#pragma once

#include "task.hpp"

extern "C"
{
    extern Task* volatile currentTaskControlBlock;
    
    void TaskScheduler();
}

void scheduler_yield();
bool SchedulerTick();
void startFirstTask() __attribute__((naked));

void DelayTask(uint32_t ticks = 0);
void DelayTask(Task& task, uint32_t ticks = 0);