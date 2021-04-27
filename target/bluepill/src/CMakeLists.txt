
if(IS_FIRMWARE)
    target_sources(bluepill PUBLIC
        main.cpp
        startup.cpp
        interrupts.cpp)
endif()