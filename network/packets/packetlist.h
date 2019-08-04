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


#ifndef __TORUS_PACKETLIST_H
#define __TORUS_PACKETLIST_H

#include "../packet.h"
#include <string>

#define PACKET_CREATE_CHARACTER Packet_0x00
#define PACKET_KR_2D_CLIENT_SEED Packet_0xef
#define PACKET_MOVEMENT_REQUEST Packet_0x02
#define PACKET_MOVEMENT_REJECT Packet_0x21
#define PACKET_MOVEMENT_ACCEPT Packet_0x22

class Client;

class Packet_0x00 : public Packet {
public:
    const t_udword length();
    const t_byte * dumps();
    void loads(const t_byte *);
    void loads(Socket * s);
    ~Packet_0x00();
private:
};

class Packet_0x02 : public Packet {
public:
    const t_udword length();
    const t_byte *dumps();
    void loads(const t_byte *);
    void loads(Socket *s);
    ~Packet_0x02();
};

class Packet_0xef : public Packet {
public:
    const t_udword length();
    const t_byte * dumps();
    void loads(const t_byte * b);
    void loads(Socket * s);
    ~Packet_0xef();
    t_udword seed() { return _seed; }
private:
    t_udword _seed;
    t_udword _client_major_version;
    t_udword _client_minor_version;
    t_udword _client_revision_version;
    t_udword _client_prototype_version;
};

class Packet_0x21 : public Packet {
public:
    const t_udword length();
    const t_byte *dumps();
    void loads(const t_byte * b);
    void loads(Socket * s);
    void set_data(t_ubyte seq, Client *client);
    ~Packet_0x21();
};

class Packet_0x22 : public Packet {
public:
    const t_udword length();
    const t_byte *dumps();
    void loads(const t_byte * b);
    void loads(Socket * s);
    void set_data(t_ubyte seq, Client *client);
    ~Packet_0x22();
};

class Packet_0x55 : public Packet { //PacketLoginComplete (game character finished loading)
public:
    const t_udword length();
    const t_byte* dumps();
    void loads(const t_byte* b);
    void loads(Socket* s);
    void set_data(t_ubyte seq, Client* client);
    Packet_0x55();
    ~Packet_0x55();
};

class Packet_0x73 : public Packet { //PacketPing
public:
    const t_udword length();
    const t_byte* dumps();
    void loads(const t_byte* b);
    void loads(Socket* s);
    void set_data(t_ubyte seq, Client* client);
    Packet_0x73();
    ~Packet_0x73();
};

class Packet_0x80 : public Packet {  //LoginCredentials & ServerListRequest
    std::string accName[30];
    std::string accPassword[30];
public:
    const t_udword length();
    const t_byte* dumps();
    void loads(const t_byte* b);
    void loads(Socket* s);
    ~Packet_0x80();
};



class Packet_0x91 : public Packet {  //LoginCredentials & ServerListRequest
    std::string accName[30];
    std::string accPassword[30];
public:
    const t_udword length();
    const t_byte* dumps();
    void loads(const t_byte* b);
    void loads(Socket* s);
    ~Packet_0x91();
};

class Packet_0xa0 : public Packet {  //ServerSelect -> disconnects from login server, connects to game server and requests character's list and client's flags
public:
    const t_udword length();
    const t_byte* dumps();
    void loads(const t_byte* b) {}
    void loads(Socket* s) {}
    Packet_0xa0();
    ~Packet_0xa0();
};

class Packet_0xa8 : public Packet {  //ServerList
public:
    const t_udword length();
    const t_byte* dumps();
    void loads(const t_byte* b) {}
    void loads(Socket* s) {}
    Packet_0xa8();
    ~Packet_0xa8();
};

#endif //__TORUS_PACKETLIST_H
