
target_sources(kernel PUBLIC
    criticalsection.cpp
    criticalsection.hpp
    scheduler.cpp
    scheduler.hpp
    stack.cpp
    stack.hpp
    task.cpp
    task.hpp
    taskList.hpp
    taskListItem.cpp
    taskListItem.hpp)

if(IS_FIRMWARE)
    target_sources(kernel PUBLIC
        basepri.hpp
        interruptMasking.hpp
        lockable.hpp
        mainthread.cpp
        mainthread.hpp
        pendSvHandler.hpp
        spinlock.hpp
        status.hpp
        utils.cpp
        utils.hpp)
endif()

add_subdirectory(container)
add_subdirectory(port)
add_subdirectory(synchronisation)
