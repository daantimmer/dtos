cmake_minimum_required(VERSION 3.0)


add_library(
	hal_stm32f103
	
	hal/port/peripherals.hpp

    target/arm/stm32f1/CMSIS/Device/ST/STM32F1xx/source/Templates/system_stm32f1xx.c
    target/arm/stm32f1/CMSIS/Device/ST/STM32F1xx/Include/system_stm32f1xx.h
)

target_compile_options(hal_stm32f103
	PRIVATE
		-Wall
		-Wextra
		-ggdb
		$<$<COMPILE_LANGUAGE:C>:-Wstrict-prototypes>
		-Warray-bounds
		-Wno-unused-const-variable
		$<$<COMPILE_LANGUAGE:CXX>:-Wno-register>)

target_include_directories(hal_stm32f103
	PUBLIC
		.)

add_subdirectory(../hal hal_header)
add_subdirectory(target/arm/stm32f1)

target_link_libraries(hal_stm32f103
	PUBLIC
		ll
		hal)