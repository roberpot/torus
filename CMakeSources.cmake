
set(CORE_FILES
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
        debug/debug.cpp     debug/debug.h
        debug/callstack.cpp debug/callstack.h)
SOURCE_GROUP(debug FILES ${DEBUG_FILES})

set(LIBRARY_FILES
        library/types.h
        library/string.cpp library/string.h
        library/errors.cpp library/errors.h
        library/vector.h
        library/stack.h
        library/queue.h
        library/map.h)
SOURCE_GROUP(library FILES ${LIBRARY_FILES})

set(NETWORK_FILES
        network/crypto.cpp network/crypto.h
        network/packet.cpp network/packet.h
        network/socket.cpp network/socket.h
        network/networkmanager.cpp network/networkmanager.h
        network/packetlist.h
        network/packetlist_0x0.cpp
        network/packetlist_0xe.cpp)
SOURCE_GROUP(network FILES ${NETWORK_FILES})

set(THREAD_FILES
        threads/thread_defines.h
        threads/thread.cpp  threads/thread.h
        threads/mutex.cpp   threads/mutex.h
        threads/cond.cpp    threads/cond.h
        threads/utils.cpp   threads/utils.h)
SOURCE_GROUP(threads FILES ${THREAD_FILES})

set(GAME_FILES
        game/account.cpp    game/account.h
        game/artifact.cpp   game/artifact.h
        game/char.cpp       game/char.h
        game/item.cpp       game/item.h
        game/uid.cpp        game/uid.h)
SOURCE_GROUP(game FILES ${GAME_FILES})

set(GAME_CHAR_FILES
        game/chars/char_props.cpp    game/chars/char_props.h
        game/chars/char_stats.cpp    game/chars/char_stats.h)
SOURCE_GROUP(game\\chars FILES ${GAME_CHAR_FILES})

set (GAME_UOFILES_FILES
        game/uo_files/map_list.cpp          game/uo_files/map_list.h
        game/uo_files/map.cpp               game/uo_files/map.h
        game/uo_files/uo_file_manager.cpp   game/uo_files/uo_file_manager.h
        game/uo_files/tiledata.cpp          game/uo_files/tiledata.h
        game/uo_files/map_point.h
        game/uo_files/idx_entry.h
        game/uo_files/map_block.h)
SOURCE_GROUP(game\\uo_files FILES ${GAME_UOFILES_FILES})

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
        ${GAME_FILES}
            ${GAME_UOFILES_FILES}
            ${GAME_CHAR_FILES}
        ${MAIN_FILES})