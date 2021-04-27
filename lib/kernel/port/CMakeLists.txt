
if(IS_TESTING)
    add_subdirectory(windows)
else()
    add_subdirectory(stm32f103)
endif()