
target_sources(rtos.elf PUBLIC
    "interruptMasking.cpp"
    "pendSvHandler.cpp"
    "stackframe.hpp"
    "stackframeinitialiser.cpp"
    "systemclock.cpp"
    "systemtick.cpp"
)