# CMakeList.txt : CMake project for rtos, include src and define
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


# Add src to this project's executable.
add_executable (${ELF}
    "src/main.cpp"
	"src/interrupts.cpp"
	"src/os/scheduler.cpp"
	"src/os/task.cpp"
	"src/os/utils.cpp"
	"src/os/criticalsection.cpp"
	"src/port/stm32f1xx/systemclock.cpp"
	"src/port/stm32f1xx/systemtick.cpp"

	"src/startup.c"
)

target_include_directories(
    ${ELF}
    PRIVATE
        "src"
		"src/port/stm32f1xx"
#	SYSTEM PRIVATE
#		"${tools}\\bin\\../lib/gcc/arm-none-eabi/7.3.1/../../../../arm-none-eabi/include/c++/7.3.1"
#		"${tools}\\bin\\../lib/gcc/arm-none-eabi/7.3.1/../../../../arm-none-eabi/include/c++/7.3.1/arm-none-eabi"
#		"${tools}\\bin\\../lib/gcc/arm-none-eabi/7.3.1/../../../../arm-none-eabi/include/c++/7.3.1/backward"
#		"${tools}\\bin\\../lib/gcc/arm-none-eabi/7.3.1/include"
#		"${tools}\\bin\\../lib/gcc/arm-none-eabi/7.3.1/include-fixed"
#		"${tools}\\bin\\../lib/gcc/arm-none-eabi/7.3.1/../../../../arm-none-eabi/include"
)

add_compile_options(
		-g3
		-ffunction-sections
		-fdata-sections
		$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
		$<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>)

target_compile_options(${ELF}
	PRIVATE
		-Wall
		-Wextra
		-g3
		$<$<COMPILE_LANGUAGE:C>:-Wstrict-prototypes>
		-Warray-bounds
		-Wno-unused-const-variable
		$<$<COMPILE_LANGUAGE:CXX>:-Wno-register>)


set_target_properties(
    ${ELF}

    PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO

        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO

    LINK_FLAGS
        "-T${PROJECT_SOURCE_DIR}/libs/hal_stm32f103/target/arm/stm32f1/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/linker/STM32F103XB_FLASH.ld \
         -mthumb \
         -g3 \
         -mtune=cortex-m3 \
         -mcpu=cortex-m3 \
         -specs=nano.specs \
         -specs=nosys.specs \
         -Wl,--gc-sections \
         -Wl,-Map=${PROJECT_NAME}.map")

		 
add_subdirectory(libs/hal_stm32f103)

target_link_libraries(${ELF}
	PUBLIC
		hal_stm32f103)

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