
cmake_minimum_required(VERSION 3.0.2)
project(torus)

set(CMAKE_CONFIGURATION_TYPES "DEBUG;RELEASE;RELWITHDEBINFO")


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

# compiler flags
## Windows flags
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4 -MP")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W4 -MP")  
    set(BOOST_ROOT "e:/boost")  #temp
    set(PostgreSQL_ROOT "e:/PostgreSQL/9.6")  #temp
    set(PQXX_ROOT  "e:/PostgreSQL/libpqxx") #temp
endif(WIN32)

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

include("CMakeSources.cmake") # Reading CMakeSources with all of the ${SOURCE_FILES}.

add_executable(torus_server ${SOURCE_FILES})

# additional libs for windows
if(WIN32)
    target_link_libraries(torus_server wsock32 ws2_32) #ws2_32.lib for windows
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Boost setup
find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

# PostgreSQL setup
find_package(PostgreSQL)
if (PostgreSQL_FOUND)
    target_link_libraries(torus_server ${PostgreSQL_LIBRARIES})
    include_directories(${PostgreSQL_INCLUDE_DIRS})
    # pqxx setup
    find_package(PQXX REQUIRED)
    if (PQXX_FOUND)
        target_link_libraries(torus_server ${PQXX_LIBRARIES})
        include_directories(${PQXX_INCLUDE_DIRECTORIES})
    endif()
endif(PostgreSQL_FOUND)
