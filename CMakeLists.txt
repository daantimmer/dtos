# CMakeList.txt : CMake project for rtos, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(rtos)

set(ELF ${PROJECT_NAME}.elf)
set(MAP ${PROJECT_NAME}.map)
set(HEX ${PROJECT_NAME}.hex)
set(LST ${PROJECT_NAME}.lst)

enable_language(ASM)

set(STARTUP_FILE startup_stm32f103xb.c)

add_subdirectory(source/target/arm/stm32f1)

# Add source to this project's executable.
add_executable (${ELF}
    "source/main.cpp"
    "source/interrupts.cpp"
    "source/os/scheduler.cpp"
    "source/os/task.cpp"
    "source/os/utils.cpp"

    "source/startup.c"
    "source/target/arm/stm32f1/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c"
)

target_link_libraries(${ELF} ll)

target_include_directories(
    ${ELF}
    PRIVATE
        "source"
        "source/target/arm/stm32f1/CMSIS/Device/ST/STM32F1xx/Include"
        "${tools}\\bin\\../lib/gcc/arm-none-eabi/7.3.1/../../../../arm-none-eabi/include/c++/7.3.1"
        "${tools}\\bin\\../lib/gcc/arm-none-eabi/7.3.1/../../../../arm-none-eabi/include/c++/7.3.1/arm-none-eabi"
        "${tools}\\bin\\../lib/gcc/arm-none-eabi/7.3.1/../../../../arm-none-eabi/include/c++/7.3.1/backward"
        "${tools}\\bin\\../lib/gcc/arm-none-eabi/7.3.1/include"
        "${tools}\\bin\\../lib/gcc/arm-none-eabi/7.3.1/include-fixed"
        "${tools}\\bin\\../lib/gcc/arm-none-eabi/7.3.1/../../../../arm-none-eabi/include"
)

set_target_properties(
    ${ELF}

    PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO

        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO

    LINK_FLAGS
        "-T${PROJECT_SOURCE_DIR}/source/target/arm/stm32f1/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/linker/STM32F103XB_FLASH.ld \
         -mthumb \
         -g3 \
         -mtune=cortex-m3 \
         -mcpu=cortex-m3 \
         -specs=nano.specs \
         -specs=nosys.specs \
         -Wl,--gc-sections \
         -Wl,-Map=${PROJECT_NAME}.map")

target_compile_options(${ELF} PUBLIC
    -Wall
    -Wextra
    -g3
    -mthumb
    -mtune=cortex-m3
    -mcpu=cortex-m3
    -mlittle-endian
    -ffunction-sections
    -fdata-sections
    $<$<COMPILE_LANGUAGE:C>:-Wstrict-prototypes>
    -Warray-bounds
    -Wno-unused-const-variable
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-register>
    -specs=nano.specs
    -specs=nosys.specs
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>)

add_definitions(
    -DSTM32F103xB
)



# TODO: Add tests and install targets if needed.
# generate the lst file
add_custom_command(
    OUTPUT ${LST}

    COMMAND
        ${CMAKE_OBJDUMP} -h -S ${ELF} > ${LST}

    DEPENDS ${ELF}
)

# create hex file
add_custom_command(
    OUTPUT ${HEX}

    COMMAND
        ${CMAKE_OBJCOPY} -j .text -j .data -O ihex ${ELF} ${HEX}

    DEPENDS ${ELF}
)

add_custom_command(
    OUTPUT "print-size-${ELF}"

    COMMAND
        ${ARM_SIZE} ${ELF}

    DEPENDS ${ELF}
)

# build the intel hex file for the device
add_custom_target(
    ${PROJECT_NAME}
    ALL
    DEPENDS ${HEX} ${LST} "print-size-${ELF}"
)

set_target_properties(
    ${PROJECT_NAME}

    PROPERTIES
        OUTPUT_NAME ${ELF}
)