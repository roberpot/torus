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


#include <cstring>
#include "socket.h"

#include "../core/errors.h"
#include "../debug/debug.h"
#include "../library/string.h"
#include "../debug/callstack.h"


Socket::Socket() {
    ADDTOCALLSTACK();
    type = SOCKETTYPE_SERVER;
#ifdef _WIN64
    //_socket = INVALID_SOCKET;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == INVALID_SOCKET) {
        THROW_ERROR(NetworkError, "Create socket failed with error: " << WSAGetLastError());
    }
    buffer = new char[1024];
#endif // _WIN64
}

#ifdef _WIN64
Socket::Socket(SOCKET s) {
    _socket = s;
    init_client_socket();
}
#endif // _WIN64

void Socket::init_client_socket() {
    ADDTOCALLSTACK();
    type = SOCKETTYPE_CLIENT;
    buffer = new char[1024];
    crypto = new Crypto();
    crypto->set_mode_login();
    read_bytes(4);
    UDWORD key;
    memcpy(&key, buffer, sizeof(UDWORD));
    crypto->set_client_key(key);
    crypto->set_mode_none();
    read_bytes();
    crypto->detect_client_keys(buffer, buffer_len);
}

void Socket::bind(const char * addr, WORD port) {
    ADDTOCALLSTACK();
    DEBUG_INFO("Binding " << addr << ":" << port);
#ifdef _WIN64
    int status;
    struct sockaddr_in sockaddr_loc;
    memset(&sockaddr_loc, 0, sizeof(struct sockaddr_in));
    sockaddr_loc.sin_family = AF_INET;
    sockaddr_loc.sin_port = htons(port);
    sockaddr_loc.sin_addr.s_addr = inet_addr(addr);
    status = ::bind(_socket, (struct sockaddr *)&sockaddr_loc, sizeof(struct sockaddr));
    if (status == SOCKET_ERROR) {
        closesocket(_socket);
        THROW_ERROR(NetworkError, "bind failed with error: " << WSAGetLastError() << ". Can not bind " << addr << ":" << port);
    }
    status = listen(_socket, SOMAXCONN);
    if (status == SOCKET_ERROR) {
        closesocket(_socket);
        THROW_ERROR(NetworkError, "listen failed with error: " << WSAGetLastError());
    }
#endif // _WIN64
#ifdef __linux__
    _socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (_socket_fd < 0) {
        throw NetworkError("ERROR opening socket");
    }
    sockaddr_in _serv_addr;
    memset(&_serv_addr, 0, sizeof(sockaddr_in));
    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_addr.s_addr = INADDR_ANY;
    _serv_addr.sin_port = htons(port);
    if (::bind(_socket_fd, (struct sockaddr *) &_serv_addr, sizeof(_serv_addr)) < 0) {
        throw NetworkError("ERROR on binding");
    }
    listen(_socket_fd, 10); // TODO: replace this magic number.
#endif //__linux__
}

bool Socket::client_pending() {
    ADDTOCALLSTACK();
#ifdef _WIN64
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(_socket, &readSet);
    timeval timeout;
    timeout.tv_sec = 0;  // Zero timeout (poll)
    timeout.tv_usec = 0;
    if(select(_socket, &readSet, NULL, NULL, &timeout) != 1){
        return false;
    }
    return true;
#endif // _WIN64
    return false;
}

Socket * Socket::get_client() {
    ADDTOCALLSTACK();
    Socket * s;
#ifdef _WIN64
    s = new Socket(accept(_socket, NULL, NULL));
#endif // _WIN64
    return s;
}

const char * Socket::get_ip() {
    ADDTOCALLSTACK();
#ifdef _WIN64
    sockaddr_in client_info;
    int addrsize = sizeof(client_info);
    getpeername(_socket, (sockaddr *)&client_info, &addrsize);
    return inet_ntoa(client_info.sin_addr);
#endif // _WIN64
}

bool Socket::data_ready() {
    ADDTOCALLSTACK();
#ifdef _WIN64
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(_socket, &readSet);
    timeval timeout;
    timeout.tv_sec = 0;  // Zero timeout (poll)
    timeout.tv_usec = 0;
    if(select(_socket, &readSet, NULL, NULL, &timeout) != 1){
        return false;
    }
    return true;
#endif // _WIN64
}

Packet * Socket::read_packet() {
    ADDTOCALLSTACK();
    Packet * p;
    read_bytes();
    p = packet_factory(buffer, buffer_len);
    p->loads(this);
    return p;
}

void Socket::write_packet(Packet * p) {
    ADDTOCALLSTACK();
#ifdef _WIN64
    unsigned int data_sended = send(_socket, p->dumps(), p->length(), 0);
    if (data_sended == SOCKET_ERROR) {
        THROW_ERROR(NetworkError, "Send failed with error: " << WSAGetLastError());
    } else if (data_sended != p->length()) {
        THROW_ERROR(NetworkError, "Send " << data_sended << " bytes, instead of " << p->length() << " bytes.");
    }
#endif // _WIN64
}

void Socket::read_bytes(unsigned int len) {
    ADDTOCALLSTACK();
#ifdef _WIN64
    buffer_len = recv(_socket, buffer, len, 0);
#endif // _WIN64
    crypto->decrypt(buffer, buffer_len);
}

void Socket::shutdown() {
    ADDTOCALLSTACK();
#ifdef _WIN64
    int status = ::shutdown(_socket, SD_SEND);
    if (status == SOCKET_ERROR) {
        closesocket(_socket);
        THROW_ERROR(NetworkError, "shutdown failed with error: " << WSAGetLastError());
    }
#endif // _WIN64
}

Socket::~Socket() {
    ADDTOCALLSTACK();
#ifdef _WIN64
    closesocket(_socket);
#endif // _WIN64
}