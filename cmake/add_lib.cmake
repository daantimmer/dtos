function(add_lib lib)

    set(TARGET ${lib})
    set(TARGET ${lib} PARENT_SCOPE)

    add_library(${TARGET} STATIC)

    # if (IS_TESTING)
        # add_target(${TARGET} ${ARGN})
    # else()
        add_subdirectory(src)
    # endif()

    target_include_directories(${TARGET} PUBLIC src)
    
    if (MSVC)
        target_compile_options(${TARGET} PRIVATE /W4  /D_CRT_SECURE_NO_WARNINGS)
    else()
        target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter)
    endif()

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
    
    target_link_libraries(${TARGET} PUBLIC ${ARGN})
endfunction()