#include "kernel/interruptMasking.hpp"
#include "kernel/port/triggerTaskSwitch.hpp"
#include "kernel/scheduler.hpp"
#include "stm32f1xx_hal.h"

extern "C" void SysTick_Handler()
{
    kernel::InterruptUnmasking interruptUnmasking;

    HAL_IncTick();

    if (SchedulerTick())
    {
        TriggerTaskSwitch();
    }
}