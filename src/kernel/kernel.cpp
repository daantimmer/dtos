#include "kernel/kernel.hpp"

#include "kernel/getkernel.hpp"
#include "kernel/mainthread.hpp"
#include "kernel/scheduler.hpp"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"

namespace
{
kernel::Kernel* kernelInstance = nullptr;

void taskIdle(Task&)
{
    while (1)
    {
        __WFE();
    }
}
}

kernel::Kernel::Kernel(MainThread& mainThread)
    : idleTask{taskIdle, {GPIOA, LL_GPIO_PIN_0}}
{
    currentTaskControlBlock = &mainThread;

    kernelInstance = this;

    idleTask.priority = UINT32_MAX - 1;

    readyTasks.insert(idleTask);
}

RunnableTask& kernel::Kernel::CurrentTask() const
{
    return *currentTaskControlBlock;
}

RunnableTask& kernel::Kernel::GetIdleTask() const
{
    return idleTask;
}

kernel::Kernel& kernel::GetKernel()
{
    return *kernelInstance;
}