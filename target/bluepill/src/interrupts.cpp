
#include "kernel/interruptMasking.hpp"
#include "kernel/port/triggerTaskSwitch.hpp"
#include "kernel/scheduler.hpp"
#include "kernel/task.hpp"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>

extern "C"
{
    void NMI_Handler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void HardFault_Handler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void MemManage_Handler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void BusFault_Handler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void UsageFault_Handler()
    {
        while (true)
        {
            assert(0);
        }
    }

    // See kernel/port/stm32f103/svcHandler.cpp
    // void __attribute__((naked)) SVC_Handler()

    void DebugMon_Handler()
    {
        while (true)
        {
            assert(0);
        }
    }

    // See kernel/port/stm32f103/systickHandler.cpp
    // void SysTick_Handler()

    void WWDG_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void PVD_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void TAMPER_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void RTC_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void FLASH_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void RCC_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void EXTI0_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void EXTI1_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void EXTI2_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void EXTI3_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void EXTI4_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void DMA1_Channel1_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void DMA1_Channel2_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void DMA1_Channel3_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void DMA1_Channel4_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void DMA1_Channel5_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void DMA1_Channel6_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void DMA1_Channel7_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void ADC1_2_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void USB_HP_CAN1_TX_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void USB_LP_CAN1_RX0_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void CAN1_RX1_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void CAN1_SCE_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void EXTI9_5_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void TIM1_BRK_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void TIM1_UP_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void TIM1_TRG_COM_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void TIM1_CC_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void TIM2_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void TIM3_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void TIM4_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void I2C1_EV_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void I2C1_ER_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void I2C2_EV_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void I2C2_ER_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void SPI1_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void SPI2_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void USART1_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void USART2_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void USART3_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void EXTI15_10_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void RTCAlarm_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
    void USBWakeUp_IRQHandler()
    {
        while (true)
        {
            assert(0);
        }
    }
}
