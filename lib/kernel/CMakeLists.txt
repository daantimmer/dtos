
add_library(kernel STATIC)

set_target_properties(
    kernel

    PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO

        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(kernel PUBLIC
    src)

target_link_libraries(kernel
    PUBLIC notstd
    PUBLIC BSP
    PUBLIC embeddedinfralibwrapper
    PUBLIC type_safe
    PUBLIC Segger)

if(IS_TESTING)
    add_executable(test_kernel)

    set_target_properties(
        test_kernel

        PROPERTIES
            C_STANDARD 11
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO

            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO)

    add_test(NAME test_kernel COMMAND test_kernel)

    target_link_libraries(test_kernel PUBLIC
        gtest gmock gmock_main
        kernel
        embeddedinfralib
        type_safe)
endif()

add_subdirectory(port)
add_subdirectory(src)
