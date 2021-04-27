
target_sources(kernel PUBLIC
    "interruptMasking.cpp"
    "pendSvHandler.cpp"
    "stackframe.hpp"
    "stackframeinitialiser.cpp"
    "systemclock.cpp"
    "systemtick.cpp"
    "triggerTaskSwitch.cpp"
    "systickHandler.cpp"
    "svcHandler.cpp"
    "waitForEvent.cpp"
    "breakpoint.cpp")