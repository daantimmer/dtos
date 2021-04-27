
if(IS_FIRMWARE)
    target_sources(kernel PUBLIC
        interruptMasking.cpp
        pendSvHandler.cpp
        stackframe.hpp
        stackframeinitialiser.cpp
        systemclock.cpp
        systemtick.cpp
        triggerTaskSwitch.cpp
        systickHandler.cpp
        svcHandler.cpp
        stack.hpp
        waitForEvent.cpp
        breakpoint.cpp)
endif()
