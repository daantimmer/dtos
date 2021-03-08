
add_source(PUBLIC
    FW
        "basepri.hpp"
        "interruptMasking.hpp"
        "lockable.hpp"
        "mainthread.cpp"
        "mainthread.hpp"
        "pendSvHandler.hpp"
        "spinlock.hpp"
        "status.hpp"
        "utils.cpp"
        "utils.hpp"
    SRC
        "criticalsection.cpp"
        "criticalsection.hpp"
        "scheduler.cpp"
        "scheduler.hpp"
        "task.cpp"
        "task.hpp"
        "taskList.hpp"
        "taskListItem.cpp"
        "taskListItem.hpp"
)

add_subdirectory(container)
add_subdirectory(port)
add_subdirectory(synchronisation)
