
target_sources(kernel PUBLIC
    triggerTaskSwitch.cpp
    triggerTaskSwitch.hpp)

# add_subdirectory(stm32f1xx)

if(IS_FIRMWARE)
    target_sources(kernel PUBLIC
        interruptMasking.hpp
        stackframeinitialiser.hpp
        systemclock.hpp
        systemtick.hpp
        waitForEvent.hpp
        breakpoint.hpp)
        
endif()
