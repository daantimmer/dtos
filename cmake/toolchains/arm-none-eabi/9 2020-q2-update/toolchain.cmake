set(BIN "C:\\Program Files (x86)\\GNU Arm Embedded Toolchain\\9 2020-q2-update\\bin" CACHE PATH "gcc9 path")

set(CMAKE_C_COMPILER   "${BIN}\\arm-none-eabi-gcc.exe" CACHE FILEPATH "gcc9")
set(CMAKE_CXX_COMPILER "${BIN}\\arm-none-eabi-g++.exe" CACHE FILEPATH "g++9")
set(CMAKE_ASM_COMPILER "${BIN}\\arm-none-eabi-g++.exe" CACHE FILEPATH "g++9")

set(ARM_SIZE "${BIN}\\arm-none-eabi-size.exe" CACHE FILEPATH "size9")

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "cortex-m")