
add_library(embeddedinfralib STATIC)

set_target_properties(
    embeddedinfralib

    PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO

        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(embeddedinfralib PUBLIC
    src)

if(IS_TESTING)
    add_executable(test_embeddedinfralib)

    set_target_properties(
        test_embeddedinfralib

        PROPERTIES
            C_STANDARD 11
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO

            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
    )

    add_test(NAME test_embeddedinfralib COMMAND test_embeddedinfralib)
    
    target_link_libraries(test_embeddedinfralib PUBLIC
        gtest gmock gmock_main
        embeddedinfralib)
endif()

add_subdirectory(src)
