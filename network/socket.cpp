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
#include <network/socket.h>

#include <core/errors.h>
#include <debug_support/debug.h>
#include <library/string.h>
#include <debug_support/callstack.h>
#include <network/packet.h>
#include <network/packets/packetlist.h>
#include <game/client.h>
#include <shell.h>

#define N2L(C, LL) \
    LL = ((C&0xff000000))>>24 | ((C&0x00ff0000))>>8  | ((C&0x0000ff00))<<8 | ((C&0x000000ff)<<24)

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
    _client = 0;
    _is_closing = false;
}

Socket::Socket(socket_t s) {
    ADDTOCALLSTACK();
    _socket = s;
    rewinded_len = 0;
    init_client_socket();
}

void Socket::init_client_socket() {
    ADDTOCALLSTACK();
    rewinded_len = 0;
    type = SOCKETTYPE_CLIENT;
    buffer = new t_byte[1024];
    crypto = new Crypto();
    crypto->set_mode_none();
    udword_t seed = _determinate_client_seed();
    crypto->set_client_seed(seed);
    _read_bytes(62);
    crypto->detect_client_keys(buffer, buffer_len);
    _rewind(buffer, 62);
    crypto->set_mode_login();
    _client = new Client(this);

    Packet_0x80 *packet_connect = new Packet_0x80();
    packet_connect->loads(this);

    if (packet_connect->is_valid_account())
    {
        DEBUG_NOTICE("Received valid account identification, proceeding to send server information.");
        Packet_0xa8* packet = new Packet_0xa8();
        write_packet(packet);
    }    
}


udword_t Socket::_determinate_client_seed() {
    ADDTOCALLSTACK();
    t_ubyte _cmd;
    (*this) >> _cmd;
    _rewind((t_byte*)&_cmd, sizeof(t_ubyte));
    udword_t seed;
    if (_cmd == 0xef) {
        DEBUG_NOTICE("Detected login seed packet.");
        PACKET_KR_2D_CLIENT_SEED * p = static_cast<PACKET_KR_2D_CLIENT_SEED *>(packet_factory(*this));
        seed = p->seed();
        DEBUG_NOTICE("Client seed: " << seed);
        delete p;
    } else {
        DEBUG_NOTICE("Detected login seed (no login seed packet).");
        (*this) >> seed;
        //N2L(seed, seed);
    }
    return seed;
}

void Socket::bind(const t_byte * addr, word_t port) {
    ADDTOCALLSTACK();
    DEBUG_INFO("Binding " << addr << ":" << port);
    dword_t status;
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
#ifdef _WINDOWS
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(_socket, &readSet);
    timeval timeout;
    timeout.tv_sec = 0;  // Zero timeout (poll)
    timeout.tv_usec = 0;
    return (select((int)_socket, &readSet, NULL, NULL, &timeout) == 1);
#endif //_WINDOWS
#ifdef __linux__
    _accepted_socket = accept(_socket, 0, 0);
    return (_accepted_socket != -1);
#endif //__linux__
}

Socket * Socket::get_socket() {
    ADDTOCALLSTACK();
    Socket * s;
#ifdef _WINDOWS
    s = new Socket(accept(_socket, 0, 0));
#endif //_WINDOWS
#ifdef __linux__
    s = new Socket(_accepted_socket);
#endif //__linux__
    return s;
}

Client * Socket::get_client() {
    ADDTOCALLSTACK();
    return _client;
}

const t_byte * Socket::get_ip() {
    ADDTOCALLSTACK();
#ifdef _WINDOWS
    sockaddr_in client_info;
    dword_t addrsize = sizeof(client_info);
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
    return (select((int)_socket, &readSet, NULL, NULL, &timeout) == 1);
}

Packet* Socket::read_packet() {
    ADDTOCALLSTACK();
    Packet* p = nullptr;
    p = packet_factory(*this);
    if (p)
    {
        TORUSSHELLECHO("Network receive(" << p->length() << ") "<< std::endl << hex_dump_buffer(p->dumps(), p->length()));
    }
    return p;
}

void Socket::write_packet(Packet * p) {
    ADDTOCALLSTACK();
    p->print("Sending ");
    //TORUSSHELLECHO("Network send(" << p->length() << ") " << std::endl << hex_dump_buffer(p->dumps(), p->length()));

#ifdef _WINDOWS
    udword_t data_sended = send(_socket, p->dumps(), p->length(), 0);
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
    delete p;
}

bool Socket::is_closing()
{
    return _is_closing;
}

void Socket::set_closing()
{
    _is_closing = true;
}

void Socket::read_string(Socket& s, std::string& str, int len)
{
    ADDTOCALLSTACK();
    s._read_bytes(len);
    str.append(s.buffer);
    /*if (sizeof(T) == 4) {
        d = (((d & 0x000000ff) << 24) | ((d & 0x0000ff00) << 8) | ((d & 0x00ff0000) >> 8) | ((d & 0xff000000) >> 24));
    }*/
}

void Socket::_read_bytes(udword_t len) {
    ADDTOCALLSTACK();
    // First check if we rewinded.
    udword_t init = 0;
    udword_t len_remaining = len;
    buffer_len = 0;
    if (rewinded_len > 0) {
        if (len_remaining < rewinded_len) {
            init = len_remaining;
        } else {
            init = rewinded_len;
        }
        memcpy(buffer, rewinded, sizeof(t_byte) * init);
        rewinded_len -= init;
        len_remaining -= init;
        if (rewinded_len == 0) {
            delete rewinded;
        } else {
            memmove(rewinded, &rewinded[init], sizeof(t_byte) * rewinded_len);
        }
    }
    if (len_remaining) {
#ifdef _WINDOWS
        buffer_len = recv(_socket, &buffer[init], len_remaining, 0);
#endif // _WINDOWS
#ifdef __linux__
        buffer_len = (udword_t)recv(_socket, &buffer[init], len_remaining, 0);
#endif //__linux__
        if (buffer_len != len_remaining) {
            THROW_ERROR(NetworkError, "Error reading socket: Expected len: " << len_remaining << ", readed: " << buffer_len);
        }
    }
    buffer_len = len;
    if (crypto && crypto->has_encryption())
    {
        crypto->decrypt(buffer, len);
    }
}

void Socket::_rewind(t_byte * b, udword_t l) {
    ADDTOCALLSTACK();
    if (rewinded_len > 0) {
        t_byte * new_rewind_buffer = new t_byte[rewinded_len + l];
        memcpy(&new_rewind_buffer[l], rewinded, sizeof(t_byte)  * rewinded_len);
        delete rewinded;
        rewinded = new_rewind_buffer;
    } else {
        rewinded = new t_byte[l];
    }
    memcpy(rewinded, b, sizeof(t_byte) * l);
    rewinded_len += l;
}

void Socket::shutdown() {
    ADDTOCALLSTACK();
#ifdef _WINDOWS
    set_closing();
    udword_t status = ::shutdown(_socket, SD_SEND);
    if (status == SOCKET_ERROR) {
        closesocket(_socket);
        THROW_ERROR(NetworkError, "shutdown failed with error: " << WSAGetLastError());
    }
#endif // _WINDOWS
#ifdef __linux__
    dword_t status = ::shutdown(_socket, SHUT_WR);
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
*
    Since 7.0.13.0 and 4.0.13.0 City Name and Building Name have length of 32 chars, also added city x,y,z,map,cliloc description and dword 0 to city structure.
    */