
add_source(PUBLIC
    FW
        "interruptMasking.hpp"
        "stackframeinitialiser.hpp"
        "systemclock.hpp"
        "systemtick.hpp"
        "waitForEvent.hpp"
        "breakpoint.hpp"
    SRC
        "triggerTaskSwitch.cpp"
        "triggerTaskSwitch.hpp"
)

add_subdirectory(stm32f1xx)
add_subdirectory(testdoubles)
