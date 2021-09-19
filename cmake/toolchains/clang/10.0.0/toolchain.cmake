# set(BIN "$ENV{ProgramFiles\(x86\)}\\GNU Arm Embedded Toolchain\\10 2021.07\\bin" CACHE PATH "gcc10 path")

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "cortex-m")

if(DEFINED ENV{GCC_ARM_TOOLCHAIN})
    set(GCC_ARM_TOOLCHAIN $ENV{GCC_ARM_TOOLCHAIN})
else()
    set(GCC_ARM_TOOLCHAIN "B:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.07/bin/..")
endif()

# LIST(APPEND CMAKE_PROGRAM_PATH ${GCC_ARM_TOOLCHAIN})

set(CLANG_TARGET_TRIPLE arm-none-eabi)
set(GCC_ARM_TOOLCHAIN_PREFIX ${CLANG_TARGET_TRIPLE})

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_ASM_COMPILER clang)
set(CMAKE_ASM_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})


set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# C/C++ toolchain
set(GCC_ARM_SYSROOT "${GCC_ARM_TOOLCHAIN}/${GCC_ARM_TOOLCHAIN_PREFIX}")
set(CMAKE_SYSROOT ${GCC_ARM_SYSROOT})
# set(CMAKE_FIND_ROOT_PATH ${GCC_ARM_SYSROOT})


include_directories(SYSTEM
    ${GCC_ARM_TOOLCHAIN}/${GCC_ARM_TOOLCHAIN_PREFIX}/include/newlib-nano
    ${GCC_ARM_TOOLCHAIN}/lib/gcc/${GCC_ARM_TOOLCHAIN_PREFIX}/10.3.1/../../../../arm-none-eabi/include
    ${GCC_ARM_TOOLCHAIN}/lib/gcc/${GCC_ARM_TOOLCHAIN_PREFIX}/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1
    ${GCC_ARM_TOOLCHAIN}/lib/gcc/${GCC_ARM_TOOLCHAIN_PREFIX}/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1/arm-none-eabi/thumb/v7-m/nofp
    ${GCC_ARM_TOOLCHAIN}/lib/gcc/${GCC_ARM_TOOLCHAIN_PREFIX}/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1/backward
    ${GCC_ARM_TOOLCHAIN}/lib/gcc/${GCC_ARM_TOOLCHAIN_PREFIX}/10.3.1/include
    ${GCC_ARM_TOOLCHAIN}/lib/gcc/${GCC_ARM_TOOLCHAIN_PREFIX}/10.3.1/include-fixed
)