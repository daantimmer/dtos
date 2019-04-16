
#pragma once

#include "task.hpp"

extern "C"
{
    extern Task* volatile currentTaskControlBlock;
    
    void TaskScheduler();
}

bool SchedulerTick();
void startFirstTask() __attribute__((naked));

void YieldTask();
void BlockTask();

void DelayTask(uint32_t ticks = 0);
void DelayTask(Task& task, uint32_t ticks = 0);