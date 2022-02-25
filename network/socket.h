/*
 * This file is part of Torus.
 * Torus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Torus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with Torus. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TORUS_SOCKET_H
#define __TORUS_SOCKET_H


#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif // _WIN32
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <library/types.h>
#include <network/packetin.h>
#include <network/packetout.h>
#include <debug_support/callstack.h>

class Client;

#ifdef _WIN64
typedef SOCKET socket_t;
typedef dword_t socklen_t;
#endif //_WIN64
#ifdef __linux__
typedef int socket_t;
#endif //__linux__

#define BUFFER_SIZE 1024

enum class ConnectionState
{
    CONNECTIONSTATE_NONE,
    CONNECTIONSTATE_LOGIN,
    CONNECTIONSTATE_SERVERLIST,
    CONNECTIONSTATE_CHARLIST,
    CONNECTIONSTATE_GAME
};

enum class ConnectionType
{
    CONNECTIONTYPE_NONE,
    CONNECTIONTYPE_CLIENT,
    CONNECTIONTYPE_LOGINSERVER,
    CONNECTIONTYPE_GAMESERVER
};

/**
 * @brief Handler for socket connections.
 *
 * Manages in/out data
 */
class Socket
{
    std::queue<PacketIn*> _packets_in_queue;
    std::queue<PacketOut*> _packets_out_queue;
    PacketIn* _current_in_packet;
    bool _is_read_closed;
    bool _is_write_closed;
    socket_t _socket;
    ConnectionState _connection_state;
    ConnectionType _connection_type;
    ConnectionType _server_type;
    sockaddr_in _connection_info;
    Client* _client;
    t_byte *_buffer;
    udword_t _seed;
    bool _seeded;
#ifdef __linux
    int _accepted_socket;
#endif

    void _init();
public:

    Socket(socket_t s, ConnectionType connection_type = ConnectionType::CONNECTIONTYPE_CLIENT,
            ConnectionType server_type = ConnectionType::CONNECTIONTYPE_LOGINSERVER);
    Socket(ConnectionType connection_type = ConnectionType::CONNECTIONTYPE_LOGINSERVER);
    virtual ~Socket();

    /**
     * @brief   Determines if there is any data ready to be processed.
     *
     * @return  True if it succeeds, false if it fails.
     */
    bool data_ready(fd_set& readSet);

    /**
     * @brief   Check if the socket is closed for reading.
     *
     * @return  True if it's closed, false otherwise.
     */
    bool is_read_closed();

    /**
     * @brief   Closes the socket for reading ( if state = true ).
     *
     * @param   state  The state.
     */

    void set_read_closed(bool state = true);
    /**
     * @brief   Check if the socket is closed for writing.
     *
     * @return  True if it's closed, false otherwise.
     */
    bool is_write_closed();  
  
    /**
     * @brief   Closes the socket for writing ( if state = true ).
     *
     * @param   state  The state.
     */
    void set_write_closed(bool state = true);

    /**
     * @brief   Closes the socket for reading and writing ( if state = true ).
     *
     * @param   state  The state.
     */
    void set_read_write_closed(bool state = true);

    /**
     * @brief   Read the available date from the _socket.
     */
    bool receive(udword_t size = 1024);

    /**
     * @brief   Queue the given packet for later sending.
     *
     * @param   packet  The packet to queue.
     */
    void write(PacketOut* packet);

    /**
     * @brief   Get the current state of the connection.
     *
     * @return  The state.
     */
    ConnectionState get_connection_state();

    /**
     * @brief   Set the current state of the connection.
     *
     * @param   connection_state  The new state.
     */
    void set_connection_state(ConnectionState connection_state);

    /**
     * @brief   Get the type of connection.
     *
     * @return  The type.
     */
    ConnectionType get_connection_type();

    ConnectionType get_server_type();
    void set_server_type(ConnectionType type);


    /**
     * @brief   Gets the socket.
     *
     * @return  Null if it fails, else the socket.
     */
    Socket* create_socket(sockaddr_in &sock_in, ConnectionType server_type);

    /**
     * @brief   Gets the internal socket.
     * 
     * @return the connection socket.
     */
    socket_t get_socket();

    /**
     * @brief   Determines there is a client pending.
     *
     * @return  True if it succeeds, false if it fails.
     */
    bool client_pending(sockaddr_in &sockin);

    /**
     * @brief   Binds the given addr and the given port.
     *
     * @param   addr    The address to bind.
     * @param   port    The port to bind.
     */
    void bind(const t_byte* addr, uword_t port);

    /**
     * @brief   Gets the IP string.
     *
     * @return  Null if it fails, else the IP string.
     */
    const t_byte *get_ip_str();

    /**
     * @brief   Gets the IP.
     * 
     * @return The int representation of the IP.
     */
    dword_t get_ip();

    /**
     * @brief Send all the outgoing queued packets 
     */
    void send_queued_packets();

    /**
     * @brief Clean all the packets stored to be processed.
     */
    void clean_incoming_packets();

    /**
     * @brief Read all the incoming queued packets.
     */
    void read_queued_packets();

    /**
     * @brief   Gets the Client using this socket.
     *
     * @return The client if any yet, nullptr otherwise.
     */
    Client* get_client();

    /**
     * @brief   Attach this socket to a Client.
     *
     * @param   client  The client to attach.
     */
    void set_client(Client *client);

    /** @brief   Shuts down this object and closes its connection. */
    void shutdown();

    void set_seed(udword_t seed);
    udword_t get_seed();
};


/*
*   Connection sequence explanation up to character selection screen:
*   - Server receives packet 0xa0 indicating the index of the selected server. - Done (read FIXME)
*   - Server sends to client the connection information for the given server Packet 0x8c:
*       int32 ip
*       int16 port
*       int32 Auth ID
*   - Client sends packet 0x91 Requesting the character list:
*       int32 Auth ID
*       byte[30]	Account Name
*       byte[30]	Password
*       *Notes: Acc and password were already receipt, but this is a double check since the login server and game server maybe different, so both should do the check.
                Auth ID is an additional security token providen for the loginserver and should be expected by the gameserver.

*   - Server sends allowed flags for the client with packet 0xb9:
*       int32 flags: (since UOSA the packet has 5 bytes, before it had 3).
*           Flags
            (
                0x01 = enable T2A features: chat button, regions;
                0x02 = enable renaissance features;
                0x04 = enable third down features;
                0x08 = enable LBR features: skills, map;
                0x10 = enable AOS features: skills, spells, map, fightbook, housing tiles;
                0x20 = enable 6th character slot;
                0x40 = enable SE features: spells, skills, map, housing tiles;
                0x80 = enable ML features: elven race, spells, skills, housing tiles;
                0x100 = enable The Eight Age splash screen;
                0x200 = enable The Ninth Age splash screen and crystal/shadow housing tiles;
                0x400 = enable The Tenth Age; 0x800 = enable increased housing and bank storage;
                0x1000 = enable 7th character slot;
                0x2000 = enable roleplay faces;
                0x4000 = trial account;
                0x8000 = non-trial (live) account;
                0x10000 = enable SA features: gargoyle race, spells, skills, housing tiles;
                0x20000 - enable HS features;
                0x40000 - enable Gothic housing tiles;
                0x80000 - enable Rustic housing tiles
            )
*
*   - Server sends character list, with packet 0xa9:
*   byte characters count
*   loop
*       byte[30] character name
*       byte[30] character password (best to send empty?)
*   endloop
*   byte cities count
*   loop
*       byte city index
*       byte[32] city name
*       byte[32] building name
*       int32_t pos x
*       int32_t pos y
*       int32_t pos z
*       int32_t map id
*       int32_t cliloc description
*       int32_t 0 (yes, a 0)
*   endloop
*   int32_t flags (for character creation)*:
        Flags
        (
            0x01 = unknown flag1;
            0x02 = overwrite configuration button;
            0x04 = limit 1 character per account;
            0x08 = enable context menus;
            0x10 = limit character slots;
            0x20 = paladin and necromancer classes, tooltips;
            0x40 = 6th character slot;
            0x80 = samurai and ninja classes;
            0x100 = elven race;
            0x200 = unknown flag2;
            0x400 = send UO3D client type (client will send 0xE1 packet);
            0x800 = unknown flag3;
            0x1000 = 7th character slot;
            0x2000 = unknown flag4;
            0x4000 = new movement system;
            0x8000 = unlock new felucca areas
        )
    int32_t last character slot**.

    *Each flag is for each feature, if you need to combine features, you need to summ flags.
        Unknown Flag1 never was sent by OSI.
        Unknown Flag 2 was added with UO:KR launch.
        Unknown Flag 3 was added sometimes between UO:KR and UO:SA launch.
        Flag 4 was added with UO:SA launch.
        All 4 flags are useless: no client reaction.
        0x8000 flag is used for unlocking new Felucca factions areas, note that you have to use "_x" versions of map/statics if you want to move through new areas.
    **Last character slot for SA 3D clients: it will highlight last character used.

*    Since 7.0.13.0 and 4.0.13.0 City Name and Building Name have length of 32 chars, also added city x,y,z,map,cliloc description and dword 0 to city structure.
*/

#endif //__TORUS_SOCKET_H
