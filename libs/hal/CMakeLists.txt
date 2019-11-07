cmake_minimum_required(VERSION 3.0)

add_library(hal INTERFACE)

#target_include_directories(hal PUBLIC inc)
#target_include_directories(hal PUBLIC CMSIS/Device/ST/STM32F1xx/Include)
#target_include_directories(hal PUBLIC CMSIS/Include)

target_include_directories(
    hal
    INTERFACE
        "."
)