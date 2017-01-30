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


#ifdef __linux__
#include <cerrno>
#include <arpa/inet.h>
#endif //__linux__
#include <cstring>
#include "socket.h"

#include "../core/errors.h"
#include "../debug/debug.h"
#include "../library/string.h"
#include "../debug/callstack.h"



Socket::Socket() {
    ADDTOCALLSTACK();
    type = SOCKETTYPE_SERVER;
#ifdef _WINDOWS
    //_socket = INVALID_SOCKET;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == INVALID_SOCKET) {
        THROW_ERROR(NetworkError, "Create socket failed with error: " << WSAGetLastError());
    }
#endif // _WINDOWS
#ifdef __linux__
    _socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (_socket < 0) {
        throw NetworkError("ERROR opening socket");
    }
#endif //__linux__
    buffer = new char[1024];
}

Socket::Socket(socket_t s) {
    ADDTOCALLSTACK();
    _socket = s;
    init_client_socket();
}

void Socket::init_client_socket() {
    ADDTOCALLSTACK();
    rewinded_len = 0;
    type = SOCKETTYPE_CLIENT;
    buffer = new t_byte[1024];
    crypto = new Crypto();
    crypto->set_mode_none();
    t_udword key;
    *this >> key;
    crypto->set_client_key(key);
    crypto->set_mode_none();
    _read_bytes(61);
    crypto->detect_client_keys(buffer, buffer_len);
    _rewind(buffer, 61);
    crypto->set_mode_login();
}

void Socket::bind(const t_byte * addr, t_word port) {
    ADDTOCALLSTACK();
    DEBUG_INFO("Binding " << addr << ":" << port);
    t_dword status;
    sockaddr_in _serv_addr;
    memset(&_serv_addr, 0, sizeof(sockaddr_in));
    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_port = htons(port);
    _serv_addr.sin_addr.s_addr = inet_addr(addr);
    status = ::bind(_socket, (sockaddr*)&_serv_addr, sizeof(sockaddr));
#ifdef _WINDOWS
    if (status == SOCKET_ERROR) {
        closesocket(_socket);
        THROW_ERROR(NetworkError, "bind failed with error: " << WSAGetLastError() << ". Can not bind " << addr << ":" << port);
    }
    status = listen(_socket, SOMAXCONN);
    if (status == SOCKET_ERROR) {
        closesocket(_socket);
        THROW_ERROR(NetworkError, "listen failed with error: " << WSAGetLastError());
    }
#endif // _WINDOWS
#ifdef __linux__
    if (status < 0) {
        THROW_ERROR(NetworkError,"bind failed with error: " << strerror(errno) << ". Can not bind " << addr << ":" << port);
    }
    status = listen(_socket, SOMAXCONN);
    if (status < 0) {
        THROW_ERROR(NetworkError, "listen failed with error: " << strerror(errno));
    }
#endif //__linux__
}

bool Socket::client_pending() {
    ADDTOCALLSTACK();
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(_socket, &readSet);
    timeval timeout;
    timeout.tv_sec = 0;  // Zero timeout (poll)
    timeout.tv_usec = 0;
    return (select(_socket, &readSet, NULL, NULL, &timeout) == 1);
}

Socket * Socket::get_client() {
    ADDTOCALLSTACK();
    Socket * s;
    s = new Socket(accept(_socket, 0, 0));
    return s;
}

const t_byte * Socket::get_ip() {
    ADDTOCALLSTACK();
#ifdef _WINDOWS
    sockaddr_in client_info;
    t_dword addrsize = sizeof(client_info);
    getpeername(_socket, (sockaddr *)&client_info, &addrsize);
    return inet_ntoa(client_info.sin_addr);
#endif // _WINDOWS
#if __linux__
    sockaddr_in client_info;
    socklen_t addrsize = sizeof(client_info);
    getpeername(_socket, (sockaddr *)&client_info, &addrsize);
    return inet_ntoa(client_info.sin_addr);
#endif //__linux__
}

bool Socket::data_ready() {
    ADDTOCALLSTACK();
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(_socket, &readSet);
    timeval timeout;
    timeout.tv_sec = 0;  // Zero timeout (poll)
    timeout.tv_usec = 0;
    return (select(_socket, &readSet, NULL, NULL, &timeout) == 1);
}

Packet * Socket::read_packet() {
    ADDTOCALLSTACK();
    Packet * p = 0;
//    p = packet_factory(this);
//    p->loads(this);
    return p;
}

void Socket::write_packet(Packet * p) {
    ADDTOCALLSTACK();
#ifdef _WINDOWS
    t_udword data_sended = send(_socket, p->dumps(), p->length(), 0);
    if (data_sended == SOCKET_ERROR) {
        THROW_ERROR(NetworkError, "Send failed with error: " << WSAGetLastError());
    } else if (data_sended != p->length()) {
        THROW_ERROR(NetworkError, "Send " << data_sended << " bytes, instead of " << p->length() << " bytes.");
    }
#endif // _WINDOWS
#ifdef __linux__
    ssize_t data_sended = send(_socket, p->dumps(), p->length(), 0);
    if (data_sended == -1) {
        THROW_ERROR(NetworkError, "Send failed");
    } else if (data_sended != p->length()) {
        THROW_ERROR(NetworkError, "Send " << data_sended << " bytes, instead of " << p->length() << " bytes.");
    }
#endif //__linux__
}

void Socket::_read_bytes(t_udword len) {
    ADDTOCALLSTACK();
    // First check if we rewinded.
    t_udword init = 0;
    if (rewinded_len > 0) {
        init = (((len)<(rewinded_len))?(len):(rewinded_len));
        memcpy(buffer, rewinded, sizeof(t_byte) * init);
        rewinded_len -= init;
        if (rewinded_len == 0) {
            delete rewinded;
        } else {
            memmove(rewinded, &rewinded[init], sizeof(t_byte) * rewinded_len);
        }
        len -= init;
    }
    if (len) {
#ifdef _WINDOWS
        buffer_len = recv(_socket, &buffer[init], len, 0);
#endif // _WINDOWS
#ifdef __linux__
        buffer_len = (t_udword)recv(_socket, &buffer[init], len, 0);
#endif //__linux__
    }
    crypto->decrypt(buffer, buffer_len + init);
}

void Socket::_rewind(t_byte * b, t_udword l) {
    ADDTOCALLSTACK();
    if (rewinded_len > 0) {
        t_byte * new_rewind_buffer = new t_byte[rewinded_len + l];
        memcpy(&new_rewind_buffer[l], rewinded, sizeof(t_byte) * rewinded_len);
        delete rewinded;
        rewinded = new_rewind_buffer;
    } else {
        rewinded = new t_byte[l];
    }
    memcpy(rewinded, b, sizeof(t_byte) * l);
}

void Socket::shutdown() {
    ADDTOCALLSTACK();
#ifdef _WINDOWS
    t_dword status = ::shutdown(_socket, SD_SEND);
    if (status == SOCKET_ERROR) {
        closesocket(_socket);
        THROW_ERROR(NetworkError, "shutdown failed with error: " << WSAGetLastError());
    }
#endif // _WINDOWS
#ifdef __linux__
    t_dword status = ::shutdown(_socket, SHUT_WR);
    if (status == -1) {
        close(_socket);
        THROW_ERROR(NetworkError, "shutdown failed with error: " << strerror(errno));
    }
#endif //__linux__
}

Socket::~Socket() {
    ADDTOCALLSTACK();
#ifdef _WINDOWS
    closesocket(_socket);
#endif // _WINDOWS
#ifdef __linux__
    close(_socket);
#endif //__linux__
}
