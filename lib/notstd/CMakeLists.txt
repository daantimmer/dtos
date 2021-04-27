
add_library(notstd STATIC)

set_target_properties(
    notstd

    PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO

        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(notstd PUBLIC
    src)

add_subdirectory(src)
