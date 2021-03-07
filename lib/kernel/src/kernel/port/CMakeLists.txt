
add_source(
    PUBLIC
        FW
            "interruptMasking.hpp"
            "stackframeinitialiser.hpp"
            "triggerTaskSwitch.cpp"
            "triggerTaskSwitch.hpp"
            "systemclock.hpp"
            "systemtick.hpp"
)

add_subdirectory(stm32f1xx)
