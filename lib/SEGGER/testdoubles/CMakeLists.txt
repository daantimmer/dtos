
if(IS_TESTING)
    target_sources(Segger INTERFACE
        SEGGER_RTT.h
        SEGGER_SYSVIEW.h)
endif()