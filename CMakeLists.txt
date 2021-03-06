cmake_minimum_required(VERSION 3.0.2)
project(torus)

set(CMAKE_CONFIGURATION_TYPES "DEBUG;RELEASE;RELWITHDEBINFO")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# compiler flags
## Windows flags
if(MSVC_VERSION)
    include("CMakeConfig_Windows.cmake")
endif(MSVC_VERSION)

## gcc flags
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC) # additional flags for gcc, msvc doesn't support them.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Wall -pthread -static-libgcc -static-libstdc++")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic -Wall -pthread -static-libgcc")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s -Wl,--whole-archive -lpthread -ldl -Wl,--no-whole-archive")
endif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)

# Setting a different DEBUG_MODE=lvl for each build type.
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:Release>:DEBUG_MODE=0>
    $<$<CONFIG:Debug>:DEBUG_MODE=5>
    $<$<CONFIG:RelWithDebInfo>:DEBUG_MODE=4> )

add_definitions(-DSOURCE_PATH="${CMAKE_SOURCE_DIR}")

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(tscp_parser parser/tscp.y ${CMAKE_CURRENT_SOURCE_DIR}/parser/tscp_parser.cpp COMPILE_FLAGS "--defines=parser/tscp_parser.h --name-prefix=tscp")
FLEX_TARGET(tscp_scanner parser/tscp.l ${CMAKE_CURRENT_SOURCE_DIR}/parser/tscp_scanner.cpp COMPILE_FLAGS "--header-file=parser/tscp_scanner.h --prefix=tscp")
ADD_FLEX_BISON_DEPENDENCY(tscp_scanner tscp_parser)

include("CMakeSources.cmake") # Reading CMakeSources with all of the ${SOURCE_FILES}.

include_directories(${CMAKE_SOURCE_DIR})

if(MSVC_VERSION)
    include_directories("include")
endif(MSVC_VERSION)

add_executable(torus_server
        ${SOURCE_FILES}
        ${BISON_tscp_parser_OUTPUTS}
        ${FLEX_tscp_scanner_OUTPUTS})

# additional libs for windows
if(WIN32)
    target_link_libraries(torus_server wsock32 ws2_32 kernel32) #ws2_32.lib for windows
    elseif(UNIX)
    target_link_libraries(torus_server pthread dl)
endif()

