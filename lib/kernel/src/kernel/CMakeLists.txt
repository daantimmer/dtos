
add_source(
    PUBLIC
        FW
            "basepri.hpp"
            "criticalsection.cpp"
            "criticalsection.hpp"
            "interruptMasking.hpp"
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
            "taskList.hpp"
            "taskListItem.cpp"
            "taskListItem.hpp"
            "utils.cpp"
            "utils.hpp"
)

add_subdirectory(container)
add_subdirectory(port)
add_subdirectory(synchronisation)
