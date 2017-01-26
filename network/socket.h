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

#include "packet.h"
#include "crypto.h"

enum SocketType {
    SOCKETTYPE_NONE = 0,
    SOCKETTYPE_CLIENT,
    SOCKETTYPE_SERVER
};

class Socket {
public:
    Socket();
#ifdef _WINDOWS
    Socket(SOCKET s);
#endif //_WINDOWS
    void init_client_socket();
    void bind(const t_byte * addr, t_word port);
    bool client_pending();
    Socket * get_client();
    const t_byte * get_ip();
    bool data_ready();
    Packet * read_packet();
    void write_packet(Packet * p);
    void read_bytes(t_udword len = 1024);
    void shutdown();
    ~Socket();
private:
    t_byte * buffer;
    t_udword buffer_len;
    SocketType type;
    Crypto * crypto;
#ifdef _WINDOWS
    SOCKET _socket;
#endif // _WINDOWS
#ifdef __linux__
    t_dword _socket_fd;
#endif //__linux__
};

#endif //__TORUS_SOCKET_H
