# function(add_target_test executable)
#     if (IS_TESTING)
#         set(TARGET test_${executable})
#         set(TARGET test_${executable} PARENT_SCOPE)
        
#         add_executable(${TARGET})
#         add_subdirectory(test)

#         if (MSVC)
#             target_compile_options(${TARGET} PRIVATE /W4 /D_CRT_SECURE_NO_WARNINGS)
#         else()
#             target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter)
#         endif()

#         target_include_directories(${TARGET} PRIVATE test)
            
#         set_target_properties(
#             ${TARGET}

#             PROPERTIES
#                 C_STANDARD 11
#                 C_STANDARD_REQUIRED YES
#                 C_EXTENSIONS NO

#                 CXX_STANDARD 17
#                 CXX_STANDARD_REQUIRED YES
#                 CXX_EXTENSIONS NO
#         )

#         add_test(NAME ${TARGET} COMMAND ${TARGET})

#         target_link_libraries(${TARGET} PRIVATE gtest gmock gmock_main ${executable} ${ARGN})
#     endif()
# endfunction()