cmake_minimum_required(VERSION 3.0.2)
project(torus)

#FIND_PACKAGE(PostgreSQL)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CONFIGURATION_TYPES Debug;Release;RelWithDebInfo)   # Tipos de build
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4 ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W4 ")
endif(WIN32)
if(gcc) # additional flags for gcc, msvc doesn't support them.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Wall -pthread -static-libgcc -static-libstdc++")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic -Wall -pthread -static-libgcc")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s -Wl,--whole-archive -lpthread -ldl -Wl,--no-whole-archive")
endif(gcc)

set(COMPILE_DEFINITIONS_RELEASE DEBUG_MODE=0)

set(CORE_FILES
        core/types.h
        core/torus.cpp core/torus.h
        core/slave.cpp core/slave.h
        core/config.cpp core/config.h
        core/errors.cpp core/errors.h)
SOURCE_GROUP(core FILES ${CORE_FILES})

set(DB_FILES
        db/sqlite3.c db/sqlite3.h)
SOURCE_GROUP(db FILES ${DB_FILES})

set(DEBUG_FILES
        debug/info.h
        debug/debug.cpp debug/debug.h
        debug/callstack.cpp debug/callstack.h)
SOURCE_GROUP(debug FILES ${DEBUG_FILES})

set(LIBRARY_FILES
        library/string.cpp library/string.h)
SOURCE_GROUP(library FILES ${LIBRARY_FILES})

set(NETWORK_FILES
        network/crypto.cpp network/crypto.h
        network/packet.cpp network/packet.h
        network/socket.cpp network/socket.h
        network/networkmanager.cpp network/networkmanager.h)
SOURCE_GROUP(network FILES ${NETWORK_FILES})

set(THREAD_FILES
        threads/thread_defines.h
        threads/thread.cpp threads/thread.h
        threads/mutex.cpp threads/mutex.h
        threads/cond.cpp threads/cond.h
        threads/utils.cpp threads/utils.h)
SOURCE_GROUP(threads FILES ${THREAD_FILES})

set(MAIN_FILES
        shell.cpp shell.h
        main.cpp)
SOURCE_GROUP(main FILES ${MAIN_FILES})


set(SOURCE_FILES
        ${CORE_FILES}
        ${DB_FILES}
        ${DEBUG_FILES}
        ${LIBRARY_FILES}
        ${NETWORK_FILES}
        ${THREAD_FILES}
        ${MAIN_FILES} )

add_definitions(-DSOURCE_PATH="${CMAKE_SOURCE_DIR}")

add_executable(torus_server ${SOURCE_FILES})
if(WIN32)
    target_link_libraries(torus_server wsock32 ws2_32) #ws2_32.lib for windows
endif()
set_target_properties(torus_server PROPERTIES COMPILE_FLAGS "/wd4820 /wd4710 /wd4365")