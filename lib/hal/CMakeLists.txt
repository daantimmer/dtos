
add_library(hal STATIC)

set_target_properties(
    hal

    PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO

        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(hal PUBLIC
    src)

target_link_libraries(hal 
    PRIVATE embeddedinfralib
    PRIVATE BSP)
