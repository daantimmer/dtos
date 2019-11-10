#pragma once

#include "stm32f1xx.h"

#include <new>

namespace elib
{
template<typename T>
struct reg
{
    virtual T& operator*()
    {
        return data;
    }

    virtual T* operator->()
    {
        return &data;
    }

    T data;
};

struct memory
{
    static memory& Instance()
    {
        static memory instance;
        return instance;
    }

    void reset()
    {
        new (&data) layout();
    }

    struct layout
    {
        reg<TIM_TypeDef> regTIM2;
        reg<TIM_TypeDef> regTIM3;
        reg<TIM_TypeDef> regTIM4;
        reg<RTC_TypeDef> regRTC;
        reg<WWDG_TypeDef> regWWDG;
        reg<IWDG_TypeDef> regIWDG;
        reg<SPI_TypeDef> regSPI2;
        reg<USART_TypeDef> regUSART2;
        reg<USART_TypeDef> regUSART3;
        reg<I2C_TypeDef> regI2C1;
        reg<I2C_TypeDef> regI2C2;
        reg<USB_TypeDef> regUSB;
        reg<CAN_TypeDef> regCAN1;
        reg<BKP_TypeDef> regBKP;
        reg<PWR_TypeDef> regPWR;
        reg<AFIO_TypeDef> regAFIO;
        reg<EXTI_TypeDef> regEXTI;
        reg<GPIO_TypeDef> regGPIOA;
        reg<GPIO_TypeDef> regGPIOB;
        reg<GPIO_TypeDef> regGPIOC;
        reg<GPIO_TypeDef> regGPIOD;
        reg<GPIO_TypeDef> regGPIOE;
        reg<ADC_TypeDef> regADC1;
        reg<ADC_TypeDef> regADC2;
        reg<ADC_Common_TypeDef> regADC12_COMMON;
        reg<TIM_TypeDef> regTIM1;
        reg<SPI_TypeDef> regSPI1;
        reg<USART_TypeDef> regUSART1;
        reg<SDIO_TypeDef> regSDIO;
        reg<DMA_TypeDef> regDMA1;
        reg<DMA_Channel_TypeDef> regDMA1_Channel1;
        reg<DMA_Channel_TypeDef> regDMA1_Channel2;
        reg<DMA_Channel_TypeDef> regDMA1_Channel3;
        reg<DMA_Channel_TypeDef> regDMA1_Channel4;
        reg<DMA_Channel_TypeDef> regDMA1_Channel5;
        reg<DMA_Channel_TypeDef> regDMA1_Channel6;
        reg<DMA_Channel_TypeDef> regDMA1_Channel7;
        reg<RCC_TypeDef> regRCC;
        reg<CRC_TypeDef> regCRC;
        reg<FLASH_TypeDef> regFLASH;
        reg<OB_TypeDef> regOB;
        reg<DBGMCU_TypeDef> regDBGMCU;
    };

    layout data;

private:
    memory()
    {
    }
};
} // namespace elib