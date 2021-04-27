# function(add_source visibility)
#     cmake_parse_arguments(
#         PRFX
#         ""
#         ""
#         "FW;SRC;TST"
#         ${ARGN}
#     )
    
#     if (PRFX_SRC)
#         target_sources(${TARGET} ${visibility} ${PRFX_SRC})
#     endif()

#     if (PRFX_FW AND IS_FIRMWARE)
#         target_sources(${TARGET} ${visibility} ${PRFX_FW})
#     endif()

#     if (PRFX_TST AND IS_TESTING)
#         target_sources(${TARGET} ${visibility} ${PRFX_TST})
#     endif()
# endfunction()