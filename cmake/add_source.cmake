function(add_source visibility)
    cmake_parse_arguments(
        PRFX
        ""
        ""
        "FW;SRC;TST"
        ${ARGN}
    )
    
    if (PRFX_SRC)
        if (TARGETLIB)
            target_sources(${TARGETLIB} ${visibility} ${PRFX_SRC})
        endif()
        if (TARGET)
            target_sources(${TARGET} ${visibility} ${PRFX_SRC})
        endif()
    endif()

    if (PRFX_FW AND IS_FIRMWARE)
        if (TARGETLIB)
            target_sources(${TARGETLIB} ${visibility} ${PRFX_FW})
        endif()
        if (TARGET)
            target_sources(${TARGET} ${visibility} ${PRFX_FW})
        endif()
    endif()

    if (PRFX_TST AND IS_TESTING)
        if (TARGET)
            target_sources(${TARGET} ${visibility} ${PRFX_TST})
        endif()
    endif()
endfunction()