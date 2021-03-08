function(add_target executable)

    if (IS_FIRMWARE)
        set(ELF ${executable}.elf)
        set(MAP ${executable}.map)
        set(HEX ${executable}.hex)
        set(LST ${executable}.lst)

        set(TARGET ${ELF})
        set(TARGET ${ELF} PARENT_SCOPE)

        add_executable(${TARGET})

        add_subdirectory(src)
        
        if (MSVC)
            target_compile_options(${TARGET} PRIVATE /W4 /WX /D_CRT_SECURE_NO_WARNINGS)
        else()
            target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter)
        endif()

        target_include_directories(${TARGET} PUBLIC src)

        set_target_properties(
            ${TARGET}

            PROPERTIES
                C_STANDARD 11
                C_STANDARD_REQUIRED YES
                C_EXTENSIONS NO

                CXX_STANDARD 17
                CXX_STANDARD_REQUIRED YES
                CXX_EXTENSIONS NO
        )

            target_link_options(${TARGET}
                PRIVATE
                    -T${PROJECT_SOURCE_DIR}/lib/target/stm32f103/arm/src/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/linker/STM32F103XB_FLASH.ld
                    -mthumb 
                    -mcpu=cortex-m3 
                    -ggdb 
                    -ffunction-sections
                    -fdata-sections
                    -specs=nano.specs 
                    -specs=nosys.specs 
                    -Wl,--gc-sections 
                    -Wl,-Map=${executable}.map)
        target_link_libraries(${TARGET} PUBLIC ${ARGN})


        # generate the lst file
        add_custom_command(
            OUTPUT ${LST}

            COMMAND
                ${CMAKE_OBJDUMP} -h -S ${TARGET} > ${LST}

            DEPENDS ${TARGET}
        )

        # create hex file
        add_custom_command(
            OUTPUT ${HEX}

            COMMAND
                ${CMAKE_OBJCOPY} -j .text -j .data -O ihex ${TARGET} ${HEX}

            DEPENDS ${TARGET}
        )

        add_custom_command(
            OUTPUT "print-size-${TARGET}"

            COMMAND
                ${ARM_SIZE} ${TARGET}

            DEPENDS ${TARGET}
        )

        # build the intel hex file for the device
        add_custom_target(
            ${executable}
            ALL
            DEPENDS ${HEX} ${LST} "print-size-${TARGET}"
        )

        set_target_properties(
            ${executable}

            PROPERTIES
                OUTPUT_NAME ${TARGET}
        )
    endif()

endfunction()