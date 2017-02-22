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

class Socket {
public:
    Socket();
    Socket(socket_t s);
    void init_client_socket();
    void bind(const t_byte * addr, t_word port);
    bool client_pending();
    bool data_ready();
    Socket * get_client();
    const t_byte * get_ip();
    Packet * read_packet();
    void write_packet(Packet * p);
    void shutdown();
    ~Socket();
    template<typename T>
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
    t_udword _determinate_client_seed();
    void _read_bytes(t_udword len = 1024);
    void _rewind(t_byte * b, t_udword l);
    t_byte * buffer, * rewinded;
    t_udword buffer_len, rewinded_len;
    SocketType type;
    Crypto * crypto;
    socket_t _socket;
#ifdef __linux__
    socket_t _accepted_socket;
#endif //__linux__
};

template<typename T>
Socket & operator>>(Socket & s, T & d) {
    ADDTOCALLSTACK();
    s._read_bytes(sizeof(T));
    memcpy((void*)&d, s.buffer, sizeof(T));
    return s;
}


#endif //__TORUS_SOCKET_H
