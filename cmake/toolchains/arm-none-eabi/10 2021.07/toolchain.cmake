set(BIN "$ENV{ProgramFiles\(x86\)}\\GNU Arm Embedded Toolchain\\10 2021.07\\bin" CACHE PATH "gcc10 path")

set(CMAKE_C_COMPILER   "${BIN}\\arm-none-eabi-gcc.exe" CACHE FILEPATH "gcc10")
set(CMAKE_CXX_COMPILER "${BIN}\\arm-none-eabi-g++.exe" CACHE FILEPATH "g++10")
set(CMAKE_ASM_COMPILER "${BIN}\\arm-none-eabi-g++.exe" CACHE FILEPATH "g++10")

set(ARM_SIZE "${BIN}\\arm-none-eabi-size.exe" CACHE FILEPATH "size10")

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "cortex-m")
