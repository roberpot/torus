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
#endif

#include "../library/types.h"
#include "packet.h"
#include "crypto.h"
#include "../debug/callstack.h"

/** @brief   Values that represent socket types. */
enum SocketType {
    SOCKETTYPE_NONE = 0,
    SOCKETTYPE_CLIENT,
    SOCKETTYPE_SERVER
};

#ifdef _WIN64
typedef SOCKET socket_t;
#endif //_WIN64
#ifdef __linux__
typedef int socket_t;
#endif //__linux__

class Client;

class Socket {
public:
    /** @brief   Default constructor. */
    Socket();
    /**
     * @brief   Constructor.
     *
     * @param   s   The socket_t to process.
     */
    Socket(socket_t s);
    /** @brief   Init client socket. */
    void init_client_socket();
    /**
     * @brief   Binds the given addr and the given port.
     *
     * @param   addr    The address to bind.
     * @param   port    The port to bind.
     */
    void bind(const t_byte * addr, t_word port);
    /**
     * @brief   Determines there is a client pending.
     *
     * @return  True if it succeeds, false if it fails.
     */
    bool client_pending();
    /**
     * @brief   Determines if there is any data ready to be processed.
     *
     * @return  True if it succeeds, false if it fails.
     */
    bool data_ready();
    /**
     * @brief   Gets the socket.
     *
     * @return  Null if it fails, else the socket.
     */
    Socket * get_socket();
    /**
     * @brief   Gets the Client attached to this Socket.
     *
     * @return  Null if it fails, else the client.
     */
    Client * get_client();
    /**
     * @brief   Gets the IP.
     *
     * @return  Null if it fails, else the IP.
     */
    const t_byte * get_ip();
    /**
     * @brief   Reads a Packet.
     *
     * @return  Null if it fails, else the packet.
     */
    Packet * read_packet();
    /**
     * @brief   Writes a Packet for later sending.
     *
     * @param p the Packet to process.
     */
    void write_packet(Packet * p);
    /** @brief   Shuts down this object and closes its connection. */
    void shutdown();
    /** @brief   Destructor. */
    ~Socket();
    template<typename T>
    /**
     * @brief   Bitwise right shift operator, used to export the data received to a Packet's buffer.
     *
     * @param   s   The Socket to process.
     * @param   d   The data to process.
     *
     * @return  The shifted result.
     */
    friend Socket & operator>>(Socket & s, T & d);
//    friend Socket & operator>>(Socket & s, t_byte d);
//    friend Socket & operator>>(Socket & s, t_word d);
//    friend Socket & operator>>(Socket & s, t_dword d);
//    friend Socket & operator>>(Socket & s, t_qword d);
//    friend Socket & operator>>(Socket & s, t_ubyte d);
//    friend Socket & operator>>(Socket & s, t_uword d);
//    friend Socket & operator>>(Socket & s, t_udword d);
//    friend Socket & operator>>(Socket & s, t_uqword d);
private:
    /**
     * @brief   Determinate client seed.
     *
     * @return  A t_udword.
     */
    t_udword _determinate_client_seed();
    /**
     * @brief   Reads the given amount of bytes and decrypts the data.
     *
     * @param   len (Optional) The length.
     */
    void _read_bytes(t_udword len = 1024);
    /**
     * @brief   Rewinds the buffer.
     *
     * @param   b   If non-null, the buffer to process.
     * @param   l   The length to process.
     */
    void _rewind(t_byte * b, t_udword l);

    t_byte * buffer;        ///< Buffer for IO data.    
    t_byte * rewinded;      ///< Copy of the buffer to handle rewinds.
    t_udword buffer_len;    ///< Lenght of the Socket's buffer.
    t_udword rewinded_len;  ///< Lenght of the rewinded buffer.
    SocketType type;        ///< Type of connection.
    Crypto * crypto;        ///< Encryption handler.
    socket_t _socket;       ///< Pointer to the socket_t.
    Client *_client;        ///< Pointer to the attached game Client.
#ifdef __linux__
    socket_t _accepted_socket;  ///< Pointer to the connection socket for linux builds.
#endif //__linux__
};

template<typename T>
Socket & operator>>(Socket & s, T & d) {
    ADDTOCALLSTACK();
    s._read_bytes(sizeof(T));
    memcpy((void*)&d, s.buffer, sizeof(T));
    if (sizeof(T) == 4) {
        d = (((d & 0x000000ff) << 24) | ((d & 0x0000ff00) << 8) | ((d & 0x00ff0000) >> 8) | ((d & 0xff000000) >> 24));
    }
    return s;
}


#endif //__TORUS_SOCKET_H
