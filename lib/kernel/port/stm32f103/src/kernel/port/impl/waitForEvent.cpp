#include "kernel/port/waitForEvent.hpp"
#include "stm32f103xb.h"

void kernel::port::WaitForEvent()
{
    __WFE();
}
