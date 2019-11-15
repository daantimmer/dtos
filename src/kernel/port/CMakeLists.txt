
target_sources(rtos.elf PUBLIC
    "interruptMasking.hpp"
    "stackframeinitialiser.hpp"
    "systemclock.hpp"
    "systemtick.hpp"
)

add_subdirectory(stm32f1xx)
