
#pragma once

#include "task.hpp"

extern "C"
{
    extern Task* volatile currentTaskControlBlock;
    
    void TaskScheduler();
}

void EnterCriticalSection();
void ExitCriticalSection();

void scheduler_yield();
void startFirstTask() __attribute__((naked));