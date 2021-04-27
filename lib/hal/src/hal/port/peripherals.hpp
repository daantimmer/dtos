#pragma once

#include "hal/memory.hpp"
#include <stm32f1xx.h>

namespace hal
{
    using TIM2_Peripheral = Memory<TIM_TypeDef, TIM2_BASE>;
    using TIM3_Peripheral = Memory<TIM_TypeDef, TIM3_BASE>;
    using TIM4_Peripheral = Memory<TIM_TypeDef, TIM4_BASE>;
    using RTC_Peripheral = Memory<RTC_TypeDef, RTC_BASE>;
    using WWDG_Peripheral = Memory<WWDG_TypeDef, WWDG_BASE>;
    using IWDG_Peripheral = Memory<IWDG_TypeDef, IWDG_BASE>;
    using SPI2_Peripheral = Memory<SPI_TypeDef, SPI2_BASE>;
    using USART2_Peripheral = Memory<USART_TypeDef, USART2_BASE>;
    using USART3_Peripheral = Memory<USART_TypeDef, USART3_BASE>;
    using I2C1_Peripheral = Memory<I2C_TypeDef, I2C1_BASE>;
    using I2C2_Peripheral = Memory<I2C_TypeDef, I2C2_BASE>;
    using USB_Peripheral = Memory<USB_TypeDef, USB_BASE>;
    using CAN1_Peripheral = Memory<CAN_TypeDef, CAN1_BASE>;
    using BKP_Peripheral = Memory<BKP_TypeDef, BKP_BASE>;
    using PWR_Peripheral = Memory<PWR_TypeDef, PWR_BASE>;
    using AFIO_Peripheral = Memory<AFIO_TypeDef, AFIO_BASE>;
    using EXTI_Peripheral = Memory<EXTI_TypeDef, EXTI_BASE>;
    using GPIOA_Peripheral = Memory<GPIO_TypeDef, GPIOA_BASE>;
    using GPIOB_Peripheral = Memory<GPIO_TypeDef, GPIOB_BASE>;
    using GPIOC_Peripheral = Memory<GPIO_TypeDef, GPIOC_BASE>;
    using GPIOD_Peripheral = Memory<GPIO_TypeDef, GPIOD_BASE>;
    using GPIOE_Peripheral = Memory<GPIO_TypeDef, GPIOE_BASE>;
    using ADC1_Peripheral = Memory<ADC_TypeDef, ADC1_BASE>;
    using ADC2_Peripheral = Memory<ADC_TypeDef, ADC2_BASE>;
    using ADC12_COMMON_Peripheral = Memory<ADC_Common_TypeDef, ADC1_BASE>;
    using TIM1_Peripheral = Memory<TIM_TypeDef, TIM1_BASE>;
    using SPI1_Peripheral = Memory<SPI_TypeDef, SPI1_BASE>;
    using USART1_Peripheral = Memory<USART_TypeDef, USART1_BASE>;
    // using SDIO_Peripheral = Memory<SDIO_TypeDef, SDIO_BASE>;
    using DMA1_Peripheral = Memory<DMA_TypeDef, DMA1_BASE>;
    using DMA1_Channel1_Peripheral = Memory<DMA_Channel_TypeDef, DMA1_Channel1_BASE>;
    using DMA1_Channel2_Peripheral = Memory<DMA_Channel_TypeDef, DMA1_Channel2_BASE>;
    using DMA1_Channel3_Peripheral = Memory<DMA_Channel_TypeDef, DMA1_Channel3_BASE>;
    using DMA1_Channel4_Peripheral = Memory<DMA_Channel_TypeDef, DMA1_Channel4_BASE>;
    using DMA1_Channel5_Peripheral = Memory<DMA_Channel_TypeDef, DMA1_Channel5_BASE>;
    using DMA1_Channel6_Peripheral = Memory<DMA_Channel_TypeDef, DMA1_Channel6_BASE>;
    using DMA1_Channel7_Peripheral = Memory<DMA_Channel_TypeDef, DMA1_Channel7_BASE>;
    using RCC_Peripheral = Memory<RCC_TypeDef, RCC_BASE>;
    using CRC_Peripheral = Memory<CRC_TypeDef, CRC_BASE>;
    using FLASH_Peripheral = Memory<FLASH_TypeDef, FLASH_R_BASE>;
    using OB_Peripheral = Memory<OB_TypeDef, OB_BASE>;
    using DBGMCU_Peripheral = Memory<DBGMCU_TypeDef, DBGMCU_BASE>;
}