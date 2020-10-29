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

#include <string>
#include <network/packet.h>

#define PACKET_CREATE_CHARACTER Packet_0x00
#define PACKET_KR_2D_CLIENT_SEED Packet_0xef
#define PACKET_MOVEMENT_REQUEST Packet_0x02
#define PACKET_MOVEMENT_REJECT Packet_0x21
#define PACKET_MOVEMENT_ACCEPT Packet_0x22

class Client;

class Packet_0x00 : public Packet {
public:
    const udword_t length();
    void loads(const t_byte *);
    void loads(Socket * s);
    ~Packet_0x00();
private:
};

class Packet_0x02 : public Packet {
public:
    const udword_t length();
    void loads(const t_byte *);
    void loads(Socket *s);
    ~Packet_0x02();
};

class Packet_0xef : public Packet {
public:
    const udword_t length();
    void loads(const t_byte*) {}
    void loads(Socket * s);
    Packet_0xef();
    ~Packet_0xef();
    udword_t seed() { return _seed; }
private:
    udword_t _seed;
    udword_t _client_major_version;
    udword_t _client_minor_version;
    udword_t _client_revision_version;
    udword_t _client_prototype_version;
};

class Packet_0x21 : public Packet {
public:
    const udword_t length();
    void loads(const t_byte*) {}
    void loads(Socket*) {}
    void set_data(t_ubyte seq, Client *client);
    ~Packet_0x21();
};

class Packet_0x22 : public Packet {
public:
    const udword_t length();
    void loads(const t_byte*) {}
    void loads(Socket*) {}
    void set_data(t_ubyte seq, Client *client);
    ~Packet_0x22();
};

class Packet_0x55 : public Packet { //PacketLoginComplete (game character finished loading)
public:
    const udword_t length();
    void loads(const t_byte*) {}
    void loads(Socket*) {}
    void set_data(t_ubyte, Client*) {}
    Packet_0x55();
    ~Packet_0x55() {}
};

class Packet_0x73 : public Packet { //PacketPing
public:
    const udword_t length();
    void loads(const t_byte*) {}
    void loads(Socket* s);
    void set_data(t_ubyte seq, Client* client);
    Packet_0x73();
    ~Packet_0x73();
};

class Packet_0x80 : public Packet {  //LoginCredentials & ServerListRequest
    std::string accName[30];
    std::string accPassword[30];
public:
    const udword_t length();
    void loads(const t_byte*) {}
    void loads(Socket* s);
    ~Packet_0x80();
};

class Packet_0x82 : public Packet {  //PacketLoginResponse
public:
    enum ResponseCode { //Response codes, copied from SphereX
        Invalid = 0x00, // no account
        InUse = 0x01, // already in use
        Blocked = 0x02, // client blocked
        BadPass = 0x03, // incorrect password
        Other = 0x04, // other (e.g. timeout)

        // the error codes below are not sent to or understood by the client,
        // and should be translated into one of the codes above
        BadVersion,     // version not permitted
        BadCharacter,   // invalid character selected
        BadAuthID,      // incorrect auth id
        BadAccount,     // bad account name (length, characters)
        BadPassword,    // bad password (length, characters)
        BadEncLength,   // bad message length
        EncUnknown,     // unknown encryption
        EncCrypt,       // crypted client not allowed
        EncNoCrypt,     // non-crypted client not allowed
        CharIdle,       // character is already ingame
        TooManyChars,   // account has too many characters
        CreationBlocked,// character creation is blocked in this moments.
        BlockedIP,      // ip is blocked
        MaxClients,     // max clients reached
        MaxGuests,      // max guests reached
        MaxPassTries,   // max password tries reached


        Success = 0xFF  // no error
    };
    const udword_t length();
    void loads(const t_byte*) {}
    void loads(Socket*) {}
    void set_data(ResponseCode code);
    ~Packet_0x82();
};

class Packet_0x91 : public Packet {  //LoginCredentials & ServerListRequest
    std::string accName[30];
    std::string accPassword[30];
public:
    const udword_t length();
    void loads(const t_byte*) {}
    void loads(Socket* s);
    ~Packet_0x91();
};

class Packet_0xa0 : public Packet {  //ServerSelect -> disconnects from login server, connects to game server and requests character's list and client's flags
public:
    const udword_t length();
    void loads(const t_byte*) {}
    void loads(Socket* s);
    Packet_0xa0();
    ~Packet_0xa0();
};

class Packet_0xa8 : public Packet {  //ServerList
public:
    const udword_t length();
    void loads(const t_byte*) {}
    void loads(Socket*) {}
    Packet_0xa8();
    ~Packet_0xa8();
};

#endif //__TORUS_PACKETLIST_H
