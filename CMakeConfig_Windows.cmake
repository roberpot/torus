

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4 -MP -MTd -openmp -wd4702 -DYY_NO_UNISTD_H")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W4 -MP -MTd")  
add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS
                -D_CRT_SECURE_NO_WARNINGS
                -D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
add_compile_definitions(_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING)