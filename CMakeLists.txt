cmake_minimum_required(VERSION 3.0.2)
project(torus)

#FIND_PACKAGE(PostgreSQL)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -Wall -pedantic -pthread -std=c++11 -static-libgcc -static-libstdc++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -Wall -pedantic -pthread -std=c11 -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s -Wl,--whole-archive -lpthread -ldl -Wl,--no-whole-archive")

set(CORE_FILES
        core/types.h
        core/torus.cpp core/torus.h
        core/slave.cpp core/slave.h
        core/config.cpp core/config.h
        core/errors.cpp core/errors.h)

set(DB_FILES
        db/sqlite3.c db/sqlite3.h)

set(DEBUG_FILES
        debug/info.h
        debug/debug.cpp debug/debug.h
        debug/callstack.cpp debug/callstack.h)

set(LIBRARY_FILES
        library/string.cpp library/string.h)

set(NETWORK_FILES
        network/crypto.cpp network/crypto.h
        network/packet.cpp network/packet.h
        network/socket.cpp network/socket.h
        network/networkmanager.cpp network/networkmanager.h)

set(THREAD_FILES
        threads/thread_defines.h
        threads/thread.cpp threads/thread.h
        threads/mutex.cpp threads/mutex.h
        threads/cond.cpp threads/cond.h
        threads/utils.cpp threads/utils.h)

set(SOURCE_FILES
        ${CORE_FILES}
        ${DB_FILES}
        ${DEBUG_FILES}
        ${LIBRARY_FILES}
        ${NETWORK_FILES}
        ${THREAD_FILES}
        shell.cpp shell.h
        main.cpp )

add_definitions(-DSOURCE_PATH="${CMAKE_SOURCE_DIR}")

add_executable(torus_release ${SOURCE_FILES})
add_executable(torus_info ${SOURCE_FILES})
add_executable(torus_msg ${SOURCE_FILES})

TARGET_COMPILE_DEFINITIONS(torus_release PUBLIC DEBUG_MODE=0)
TARGET_COMPILE_DEFINITIONS(torus_info PUBLIC DEBUG_MODE=4)
TARGET_COMPILE_DEFINITIONS(torus_msg PUBLIC DEBUG_MODE=5)
