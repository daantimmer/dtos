cmake_minimum_required (VERSION 3.17.3)

project(embeddedinfralibwrapper ASM C CXX)

set(CMAKE_CXX_CLANG_TIDY "")

file(GLOB SOURCES
    embeddedinfralib/infra/event/*.cpp
    embeddedinfralib/infra/stream/*.cpp
    embeddedinfralib/infra/timer/*.cpp
    embeddedinfralib/infra/util/*.cpp

    embeddedinfralib/hal/interfaces/AnalogToDigitalPin.hpp
    
    embeddedinfralib/hal/interfaces/Gpio.cpp
    embeddedinfralib/hal/interfaces/Gpio.hpp
)

add_library(embeddedinfralibwrapper STATIC EXCLUDE_FROM_ALL ${SOURCES})

add_subdirectory(ext/infra/util/ext)

target_include_directories(embeddedinfralibwrapper SYSTEM
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/embeddedinfralib
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ext)
