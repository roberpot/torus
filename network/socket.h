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


#ifdef _WIN64
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#endif // _WIN64
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include "../core/types.h"
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
#ifdef _WIN64
    Socket(SOCKET s);
#endif //_WIN64
    void init_client_socket();
    void bind(const char * addr, WORD port);
    bool client_pending();
    Socket * get_client();
    const char * get_ip();
    bool data_ready();
    Packet * read_packet();
    void write_packet(Packet * p);
    void read_bytes(unsigned int len = 1024);
    void shutdown();
    ~Socket();
private:
    char * buffer;
    unsigned int buffer_len;
    SocketType type;
    Crypto * crypto;
#ifdef _WIN64
    SOCKET _socket;
#endif // _WIN64
#ifdef __linux__
    int _socket_fd;
#endif //__linux__
};

#endif //__TORUS_SOCKET_H
