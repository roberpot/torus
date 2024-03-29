
set(CORE_FILES
        core/torus.cpp core/torus.h
        core/slave.cpp core/slave.h
        core/config.cpp core/config.h
        core/errors.cpp core/errors.h)
SOURCE_GROUP(core FILES ${CORE_FILES})

set(CRYPT_FILES
        crypt/crypt_entry.cpp crypt/crypt_entry.h
        crypt/game_crypt.cpp crypt/game_crypt.h
        crypt/login_crypt.cpp crypt/login_crypt.h
        crypt/md5.cpp crypt/md5.h)
SOURCE_GROUP(crypt FILES ${CRYPT_FILES})

set(DB_FILES
        )
SOURCE_GROUP(db FILES ${DB_FILES})

set(DEBUG_FILES
        debug_support/info.h
        debug_support/debug.cpp debug_support/debug.h
        debug_support/callstack.cpp debug_support/callstack.h)
SOURCE_GROUP(debug_support FILES ${DEBUG_FILES})

set(LIBRARY_FILES
        library/types.h
        library/string.cpp library/string.h
        library/errors.cpp library/errors.h
        library/memory.cpp library/memory.h
        library/vector.h
        library/stack.h
        library/queue.h
        library/map.h
        library/utility.h
        library/system_headers.h)
SOURCE_GROUP(library FILES ${LIBRARY_FILES})

set(NETWORK_FILES
        network/crypto.cpp              network/crypto.h
        network/huffman_compression.cpp network/huffman_compression.h
        network/packetin.cpp            network/packetin.h
        network/packetout.cpp           network/packetout.h
        network/packet.cpp              network/packet.h
        network/socket.cpp              network/socket.h
        network/networkmanager.cpp      network/networkmanager.h)
SOURCE_GROUP(network FILES ${NETWORK_FILES})

set(PACKETLIST_FILES
        network/packets/packetlist.h)
SOURCE_GROUP(network\\packets FILES ${PACKETLIST_FILES})

set(PACKETS_IN_FILES
        network/packets/in/packet_0x0.cpp
        network/packets/in/packet_0x02.cpp
        network/packets/in/packet_0x03.cpp
        network/packets/in/packet_0x06.cpp
        network/packets/in/packet_0x09.cpp
        network/packets/in/packet_0x34.cpp
        network/packets/in/packet_0x5d.cpp
        network/packets/in/packet_0x6c.cpp
        network/packets/in/packet_0x73.cpp
        network/packets/in/packet_0x80.cpp
        network/packets/in/packet_0x91.cpp
        network/packets/in/packet_0xa0.cpp
        network/packets/in/packet_0xad.cpp
        network/packets/in/packet_0xbd.cpp
        network/packets/in/packet_0xbf.cpp
        network/packets/in/packet_0xef.cpp
        )
SOURCE_GROUP(network\\packets\\in FILES ${PACKETS_IN_FILES})

set(PACKETS_OUT_FILES
        network/packets/out/packet_0x11.cpp
        network/packets/out/packet_0x17.cpp
        network/packets/out/packet_0x1b.cpp
        network/packets/out/packet_0x1c.cpp
        network/packets/out/packet_0x20.cpp
        network/packets/out/packet_0x21.cpp
        network/packets/out/packet_0x22.cpp
        network/packets/out/packet_0x3a.cpp
        network/packets/out/packet_0x55.cpp
        network/packets/out/packet_0x6c.cpp
        network/packets/out/packet_0x6d.cpp
        network/packets/out/packet_0x73.cpp
        network/packets/out/packet_0x77.cpp
        network/packets/out/packet_0x78.cpp
        network/packets/out/packet_0x82.cpp
        network/packets/out/packet_0x88.cpp
        network/packets/out/packet_0x8c.cpp
        network/packets/out/packet_0xa8.cpp
        network/packets/out/packet_0xa9.cpp
        network/packets/out/packet_0xb9.cpp
        network/packets/out/packet_0xbf.cpp
        network/packets/out/packet_0xf3.cpp
)
SOURCE_GROUP(network\\packets\\out FILES ${PACKETS_OUT_FILES})

set(PARSER_FILES
        parser/ast/ast.cpp parser/ast/ast.h
        parser/compiler.cpp parser/compiler.h)
SOURCE_GROUP(parser FILES ${PARSER_FILES})

set(THREAD_FILES
        threads/thread_defines.h
        threads/thread.cpp  threads/thread.h
        threads/mutex.cpp   threads/mutex.h
        threads/cond.cpp    threads/cond.h
        threads/utils.cpp   threads/utils.h)
SOURCE_GROUP(threads FILES ${THREAD_FILES})

set(GAME_FILES
        game/account.cpp            game/account.h
        game/accounts_manager.cpp   game/accounts_manager.h
        game/artifact.cpp           game/artifact.h
        game/client.cpp             game/client.h
        game/coord_point.cpp        game/coord_point.h
        game/char.cpp               game/char.h
        game/enums.h
        game/item.cpp               game/item.h
        game/server.cpp             game/server.h
        game/uid.cpp                game/uid.h
        game/word_object_container.cpp game/word_object_container.h)
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

set (TORUS_PARSER_FILES
        parser/ast/arithmetic.cpp parser/ast/arithmetic.h
        parser/ast/ast_garbage_collector.cpp parser/ast/ast_garbage_collector.h
        parser/ast/resources.cpp parser/ast/resources.h
        parser/ast/defname.cpp parser/ast/defname.h
        parser/ast/base.cpp parser/ast/base.h
        parser/ast/ast.cpp parser/ast/ast.h
        parser/ast/visitor.cpp parser/ast/visitor.h
        parser/tscp.l
        parser/tscp.y)
SOURCE_GROUP(parser FILES ${TORUS_PARSER_FILES})
SOURCE_GROUP(parser FILES ${BISON_tscp_parser_OUTPUTS})
SOURCE_GROUP(parser FILES ${FLEX_tscp_scanner_OUTPUTS})

set(MAIN_FILES
        shell.cpp shell.h
        main.cpp)
SOURCE_GROUP(main FILES ${MAIN_FILES})

set(TPU_FILES
        tpu/data.cpp tpu/data.h
        tpu/defname.cpp tpu/defname.h
        tpu/symbol.cpp tpu/symbol.h)
SOURCE_GROUP(tpu FILES ${TPU_FILES})

set(SOURCE_FILES
        ${CORE_FILES}
        ${DB_FILES}
        ${LIBPQXX_SOURCES}
        ${DEBUG_FILES}
        ${LIBRARY_FILES}
        ${NETWORK_FILES}
        ${PACKETLIST_FILES}
        ${PACKETS_IN_FILES}
        ${PACKETS_OUT_FILES}
        ${PARSER_FILES}
        ${THREAD_FILES}
        ${GAME_FILES}
        ${GAME_UOFILES_FILES}
        ${GAME_CHAR_FILES}
        ${TORUS_PARSER_FILES}
        ${MAIN_FILES}
        ${TPU_FILES})