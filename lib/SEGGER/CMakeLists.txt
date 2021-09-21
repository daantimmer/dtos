

if(IS_FIRMWARE)
    add_library(Segger STATIC)

    target_include_directories(Segger PUBLIC
        Config
        RTT
        SystemView
    )
    
    add_subdirectory(RTT)
    add_subdirectory(SystemView)
else()
    add_library(Segger INTERFACE)
    add_subdirectory(testdoubles)
    target_include_directories(Segger
        INTERFACE testdoubles
        INTERFACE Config)
endif()