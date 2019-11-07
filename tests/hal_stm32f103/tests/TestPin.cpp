#include "gmock/gmock.h"
#include <port/pin.hpp>

#include "stm32f1xx_ll_gpio_spy.hpp"

TEST(TestInputPin, CanSetAsInput)
{
    LL_GPIO_Spy llGpioSpy {};
    hal::port::PinInput inputPin;

	EXPECT_CALL(llGpioSpy, LL_GPIO_SetPinMode(::testing::_, ::testing::_, ::testing::_)).Times(1);

	inputPin.SetInput();
}
