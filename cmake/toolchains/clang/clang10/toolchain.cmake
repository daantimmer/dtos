
set(GCC_9_BIN "E:\\git\\toolchains\\gcc-arm-none-eabi-9-2019-q4-major-win32\\bin\\")

set(CMAKE_C_COMPILER   ${GCC_9_BIN}"arm-none-eabi-gcc.exe")
set(CMAKE_CXX_COMPILER ${GCC_9_BIN}"arm-none-eabi-g++.exe")
set(CMAKE_ASM_COMPILER ${GCC_9_BIN}"arm-none-eabi-g++.exe")

set(ARM_SIZE ${GCC_9_BIN}"arm-none-eabi-size.exe")

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "cortex-m")
