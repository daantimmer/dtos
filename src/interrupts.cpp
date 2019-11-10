
#include "kernel/InterruptMasking.hpp"
#include "kernel/scheduler.hpp"
#include "kernel/task.hpp"
// #include "kernel/utils.hpp"
#include "stm32f103xb.h"
#include "stm32f1xx_ll_gpio.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>

constexpr std::uint32_t MAIN_RETURN{0xFFFFFFF9ul};   //Tells the handler to return using the MSP
constexpr std::uint32_t THREAD_RETURN{0xFFFFFFFDul}; //Tells the handler to return using the PSP

constexpr std::uint32_t MAX_SYSCALL_INTERRUPT_PRIORITY = 1;

extern "C"
{
    void NMI_Handler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void HardFault_Handler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void MemManage_Handler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void BusFault_Handler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void UsageFault_Handler()
    {
        while (1)
        {
            assert(0);
        }
    }

    void __attribute__((naked)) SVC_Handler()
    {
        asm volatile("ldr r3, currentTaskControlBlockSVC       \n"
                     "ldr r1, [r3]                               \n"
                     "ldr r0, [r1]                               \n"
                     "ldmia r0!, {r4-r11}                        \n"
                     "msr psp, r0                                \n"
                     "isb                                        \n"
                     "orr lr, #0xd                               \n"
                     "bx lr                                      \n"
                     ".align 4                                   \n"
                     "currentTaskControlBlockSVC: .word currentTaskControlBlock");
    }

    void DebugMon_Handler()
    {
        while (1)
        {
            assert(0);
        }
    }

    void __attribute__((naked)) PendSV_Handler()
    {
        asm volatile(
        "mrs r0, psp\n"
        "isb\n"
        "\n"
        "ldr r3, currentTaskControlBlockPendSV\n"
        "ldr r2, [r3]\n"
        "\n"
        "stmdb r0!, {r4-r11}\n"
        "str r0, [r2]\n"
        "\n"
        "stmdb sp!, {r3, r14}\n"
        "mov r0, %0\n"
        "msr basepri, r0\n"
        "bl TaskScheduler\n"
        "mov r0, #0\n"
        "msr basepri, r0\n"
        "ldmia sp!, {r3, r14}\n"
        "\n"
        "ldr r1, [r3]\n"
        "ldr r0, [r1]\n"
        "ldmia r0!, {r4-r11}\n"
        "msr psp, r0\n"
        "isb\n"
        "bx lr\n"
        "\n"
        ".align 4                                   \n"
        "currentTaskControlBlockPendSV: .word currentTaskControlBlock" ::"i"(MAX_SYSCALL_INTERRUPT_PRIORITY));
    }

    void SysTick_Handler()
    {
        rtos::InterruptUnmasking interruptUnmasking;
        // DisableInterrupts();

        if (SchedulerTick() == true)
        {
            YieldTask();
        }

        // EnableInterrupts();
    }

    void WWDG_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void PVD_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void TAMPER_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void RTC_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void FLASH_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void RCC_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void EXTI0_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void EXTI1_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void EXTI2_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void EXTI3_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void EXTI4_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void DMA1_Channel1_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void DMA1_Channel2_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void DMA1_Channel3_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void DMA1_Channel4_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void DMA1_Channel5_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void DMA1_Channel6_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void DMA1_Channel7_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void ADC1_2_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void USB_HP_CAN1_TX_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void USB_LP_CAN1_RX0_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void CAN1_RX1_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void CAN1_SCE_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void EXTI9_5_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void TIM1_BRK_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void TIM1_UP_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void TIM1_TRG_COM_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void TIM1_CC_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void TIM2_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void TIM3_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void TIM4_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void I2C1_EV_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void I2C1_ER_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void I2C2_EV_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void I2C2_ER_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void SPI1_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void SPI2_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void USART1_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void USART2_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void USART3_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void EXTI15_10_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void RTCAlarm_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
    void USBWakeUp_IRQHandler()
    {
        while (1)
        {
            assert(0);
        }
    }
}
