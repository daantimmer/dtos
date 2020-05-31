#include "kernel/kernel.hpp"
#include "kernel/getkernel.hpp"
#include "kernel/mainthread.hpp"
#include "kernel/scheduler.hpp"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"

namespace
{
    kernel::Kernel* kernelInstance = nullptr;

    void taskIdle(const Task& /*unused*/)
    {
        while (true)
        {
            __WFE();
        }
    }
}

kernel::Kernel::Kernel(MainThread& mainThread)
    : idleTask{taskIdle, {GPIOA, LL_GPIO_PIN_0}} //NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
{
    currentTaskControlBlock = &mainThread;

    kernelInstance = this;

    idleTask.priority = UINT32_MAX - 1;

    readyTasks.insert(idleTask);
}

auto kernel::Kernel::CurrentTask() -> RunnableTask&
{
    return *currentTaskControlBlock;
}

auto kernel::Kernel::GetIdleTask() const -> RunnableTask&
{
    return idleTask;
}

auto kernel::GetKernel() -> kernel::Kernel&
{
    return *kernelInstance;
}