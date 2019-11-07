
#include "stm32f1xx_ll_gpio.h"

#include "stm32f1xx_ll_gpio_spy.hpp"

#include <assert.h>
#include <optional>

namespace
{
    ::testing::StrictMock<LL_GPIO_SpyBase>& InvalidSpy()
    {
        static ::testing::StrictMock<LL_GPIO_SpyBase> invalidSpy;
        return invalidSpy;
    }

    std::optional<LL_GPIO_SpyBase*> instance;
}

LL_GPIO_Spy::LL_GPIO_Spy()
{
    assert(!instance.has_value());

    instance = this;
}

LL_GPIO_Spy::~LL_GPIO_Spy()
{
    instance.reset();
}

void LL_GPIO_SetPinMode(GPIO_TypeDef* GPIOx, uint32_t Pin, uint32_t Mode)
{
    instance.value_or(&InvalidSpy())->LL_GPIO_SetPinMode(GPIOx, Pin, Mode);
}

void LL_GPIO_ResetOutputPin(GPIO_TypeDef* GPIOx, uint32_t PinMask)
{
    instance.value_or(&InvalidSpy())->LL_GPIO_ResetOutputPin(GPIOx, PinMask);
}

void LL_GPIO_SetOutputPin(GPIO_TypeDef* GPIOx, uint32_t PinMask)
{
    instance.value_or(&InvalidSpy())->LL_GPIO_SetOutputPin(GPIOx, PinMask);
}

TEST(LL_GPIO_Spy, SetPinMode)
{
    LL_GPIO_Spy sut;

    GPIO_TypeDef gpioTypeDef1;
    GPIO_TypeDef gpioTypeDef2;

    EXPECT_CALL(sut, LL_GPIO_SetPinMode(&gpioTypeDef1, 10, 15)).Times(1);
    EXPECT_CALL(sut, LL_GPIO_SetPinMode(&gpioTypeDef2, 30, 512)).Times(1);

    LL_GPIO_SetPinMode(&gpioTypeDef1, 10, 15);
    LL_GPIO_SetPinMode(&gpioTypeDef2, 30, 512);
}

TEST(LL_GPIO_Spy, SetPinModeNoSpy)
{
    EXPECT_CALL(InvalidSpy(), LL_GPIO_SetPinMode(nullptr, 0, 0)).Times(1);
    LL_GPIO_SetPinMode(nullptr, 0, 0);
}

TEST(LL_GPIO_Spy, ResetOutputPin)
{
    LL_GPIO_Spy sut;

    GPIO_TypeDef gpioTypeDef1;
    GPIO_TypeDef gpioTypeDef2;

    EXPECT_CALL(sut, LL_GPIO_ResetOutputPin(&gpioTypeDef1, 11)).Times(1);
    EXPECT_CALL(sut, LL_GPIO_ResetOutputPin(&gpioTypeDef2, 31)).Times(1);

    LL_GPIO_ResetOutputPin(&gpioTypeDef1, 11);
    LL_GPIO_ResetOutputPin(&gpioTypeDef2, 31);
}

TEST(LL_GPIO_Spy, ResetOutputPinNoSpy)
{
    EXPECT_CALL(InvalidSpy(), LL_GPIO_ResetOutputPin(nullptr, 0)).Times(1);
    LL_GPIO_ResetOutputPin(nullptr, 0);
}

TEST(LL_GPIO_Spy, SetOutputPin)
{
    LL_GPIO_Spy sut;

    GPIO_TypeDef gpioTypeDef1;
    GPIO_TypeDef gpioTypeDef2;

    EXPECT_CALL(sut, LL_GPIO_SetOutputPin(&gpioTypeDef1, 12)).Times(1);
    EXPECT_CALL(sut, LL_GPIO_SetOutputPin(&gpioTypeDef2, 32)).Times(1);

    LL_GPIO_SetOutputPin(&gpioTypeDef1, 12);
    LL_GPIO_SetOutputPin(&gpioTypeDef2, 32);
}

TEST(LL_GPIO_Spy, SetOutputPinNoSpy)
{
    EXPECT_CALL(InvalidSpy(), LL_GPIO_SetOutputPin(nullptr, 0)).Times(1);
    LL_GPIO_SetOutputPin(nullptr, 0);
}
