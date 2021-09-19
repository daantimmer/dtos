
set(executable bluepill)
set(MAP ${executable}.map)
set(HEX ${executable}.hex)
set(LST ${executable}.lst)

if(IS_FIRMWARE)
    add_executable(bluepill)
else()
    add_library(bluepill STATIC)
    add_executable(test_bluepill)

    set_target_properties(
        test_bluepill

        PROPERTIES
            C_STANDARD 11
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO

            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
    )
endif()

set_target_properties(
    bluepill

    PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO

        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

if(IS_FIRMWARE)
    target_include_directories(bluepill PUBLIC
        src)

    target_link_options(bluepill PUBLIC
        -T${PROJECT_SOURCE_DIR}/lib/target/bluepill/STM32F103C8TX_FLASH.ld
        -Wl,--print-memory-usage
        -Wl,--gc-sections 
        -Wl,-Map=bluepill.map
    )
endif()

add_subdirectory(src)

target_link_libraries(bluepill PUBLIC
    # embeddedinfralib
    kernel
    hal
    # Segger
    # PRIVATE BSP
    )

if (IS_TESTING)
    add_subdirectory(test)

    target_link_libraries(test_bluepill PUBLIC
        bluepill
        gtest
        gmock
        gmock_main
    )
endif()

if(IS_FIRMWARE)
    # generate the lst file
    add_custom_command(
        OUTPUT ${LST}

        COMMAND
            ${CMAKE_OBJDUMP} -h -S bluepill > ${LST}

        DEPENDS bluepill
    )

    # create hex file
    add_custom_command(
        OUTPUT ${HEX}

        COMMAND
            ${CMAKE_OBJCOPY} -j .text -j .data -O ihex bluepill ${HEX}

        DEPENDS bluepill
    )

    add_custom_command(
        OUTPUT "print-size-bluepill"

        COMMAND
            ${ARM_SIZE} bluepill

        DEPENDS bluepill
    )

    # build the intel hex file for the device
    add_custom_target(
        bluepill.size
        ALL
        DEPENDS ${HEX} ${LST} "print-size-bluepill"
    )

    set_target_properties(
        bluepill.size

        PROPERTIES
            OUTPUT_NAME bluepill
    )
endif()