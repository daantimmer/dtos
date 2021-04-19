cmake_minimum_required (VERSION 3.17.3)

project(BluepillBSP C ASM)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS
    # *.s
    *.h
    *.c)

set(VENDOR ST)
set(FAMILY STM32F1xx)
set(CHIP STM32F103xB)

add_library(BSP OBJECT EXCLUDE_FROM_ALL ${sources})

target_compile_definitions(BSP PUBLIC ${CHIP})

target_include_directories(BSP SYSTEM PUBLIC
    Core/Inc
    Drivers/CMSIS/Device/${VENDOR}/${FAMILY}/Include
    Drivers/CMSIS/Include
    Drivers/${FAMILY}_HAL_Driver/Inc
    Middlewares/${VENDOR}/STM32_USB_Device_Library/Class/CDC/Inc
    Middlewares/${VENDOR}/STM32_USB_Device_Library/Core/Inc
    USB_DEVICE/App
    USB_DEVICE/Target
)

set_target_properties(BSP
    PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO

        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
