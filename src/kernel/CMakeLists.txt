
target_sources(rtos.elf PUBLIC
    "basepri.hpp"
    "criticalsection.cpp"
    "criticalsection.hpp"
    "interruptMasking.hpp"
    "kernel.cpp"
    "kernel.hpp"
    "lockable.hpp"
    "mainthread.cpp"
    "mainthread.hpp"
    "pendSvHandler.hpp"
    "scheduler.cpp"
    "scheduler.hpp"
    "spinlock.hpp"
    "status.hpp"
    "task.cpp"
    "task.hpp"
    "utils.cpp"
    "utils.hpp"
)

add_subdirectory(port)
