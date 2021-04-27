# CMakeList.txt : CMake project for rtos, include src and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.17.3)

project(bluepill C CXX ASM)

option(UNITTEST "build for unit testing" OFF)
option(ACCEPTANCETEST "build for integration testing" OFF)

if (UNITTEST)
    set(IS_FIRMWARE NO)
    set(IS_TESTING YES)
else()
    set(IS_FIRMWARE YES)
    set(IS_TESTING NO)

    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

include(cmake/add_source.cmake)
include(cmake/add_target.cmake)
include(cmake/add_target_test.cmake)
include(cmake/add_lib.cmake)

# if (IS_TESTING)
#     set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-p=compile_commands.json;--extra-arg=-DCLANG_TIDY")
# else()
#     set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-p=compile_commands.json;--extra-arg=-DCLANG_TIDY;--extra-arg=-D__GNUC__;--extra-arg=-DSTM32F103xB")
# endif()


if(IS_TESTING)
    add_subdirectory(lib/gtest)
endif()

if (IS_FIRMWARE)
    add_compile_options(
            -mthumb
            -mcpu=cortex-m3
            -ggdb

            -ffunction-sections
            -fdata-sections

            -specs=nano.specs 
            -specs=nosys.specs 
            $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
            $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    )

    add_link_options(
            -mthumb 
            -mcpu=cortex-m3 
            -ggdb 

            -ffunction-sections
            -fdata-sections

            -specs=nano.specs 
            -specs=nosys.specs 
            $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
            $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    )

endif()

add_subdirectory(lib/notstd)
add_subdirectory(lib/target/bluepill)
add_subdirectory(lib/type_safe)
add_subdirectory(lib/SEGGER)
add_subdirectory(lib/embeddedinfralib)
add_subdirectory(lib/kernel)
add_subdirectory(lib/hal)
add_subdirectory(target/bluepill)