#ifndef HARNESS_TARGET_STM32F1XX_LL_GPIO_SPY_H
#define HARNESS_TARGET_STM32F1XX_LL_GPIO_SPY_H

#include <cstdint>
#include <gmock/gmock.h>
#include <optional>
#include "stm32f1xx_ll_gpio.h"

struct LL_GPIO_SpyBase
{
    MOCK_METHOD3(LL_GPIO_SetPinMode, void(GPIO_TypeDef*, uint32_t, uint32_t));
    MOCK_METHOD2(LL_GPIO_ResetOutputPin, void(GPIO_TypeDef*, uint32_t));
    MOCK_METHOD2(LL_GPIO_SetOutputPin, void(GPIO_TypeDef*, uint32_t));

protected:
    LL_GPIO_SpyBase()
    {
    }
    ~LL_GPIO_SpyBase()
    {
    }
};

struct LL_GPIO_Spy : LL_GPIO_SpyBase
{
    LL_GPIO_Spy();
    ~LL_GPIO_Spy();
};

#endif
