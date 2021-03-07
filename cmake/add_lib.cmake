function(add_lib lib)

    set(TARGETLIB ${lib})
    set(TARGETLIB ${lib} PARENT_SCOPE)

    add_library(${TARGETLIB} STATIC)

    if (IS_TESTING)
        add_target(${TARGETLIB} ${ARGN})
    else()
        add_subdirectory(src)
    endif()

    target_include_directories(${TARGETLIB} PUBLIC src)
    
    if (MSVC)
        target_compile_options(${TARGETLIB} PRIVATE /W4 /WX)
    else()
        target_compile_options(${TARGETLIB} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter)
    endif()

    set_target_properties(
        ${TARGETLIB}

        PROPERTIES
            C_STANDARD 11
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO

            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
    )
    
    target_link_libraries(${TARGETLIB} PUBLIC ${ARGN})
endfunction()