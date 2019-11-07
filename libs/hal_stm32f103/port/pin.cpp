#include "pin.hpp"

#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"

bool hal::port::PinInput::Get() const
{
    return false;
}

void hal::port::PinInput::SetInput()
{
    LL_GPIO_SetPinMode(nullptr, 0, LL_GPIO_MODE_INPUT);
}

void hal::port::PinOutput::Set(bool)
{
}

void hal::port::PinOutput::SetOutput()
{
}

void hal::port::PinOutput::SetOutput(bool)
{
}
