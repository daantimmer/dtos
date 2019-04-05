
set(tools "E:/GNU Tools Arm Embedded/7 2018-q2-update")

set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER    ${tools}/bin/arm-none-eabi-gcc.exe)
set(CMAKE_CXX_COMPILER  ${tools}/bin/arm-none-eabi-g++.exe)
set(CMAKE_ASM_COMPILER  ${tools}/bin/arm-none-eabi-g++.exe)
set(ARM_SIZE            ${tools}/bin/arm-none-eabi-size.exe)
