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
#include <network/packetin.h>
#include <network/packetout.h>

#define PACKET_CREATE_CHARACTER Packet_0x00
#define PACKET_KR_2D_CLIENT_SEED Packet_0xef
#define PACKET_MOVEMENT_REQUEST Packet_0x02
#define PACKET_MOVEMENT_REJECT Packet_0x21
#define PACKET_MOVEMENT_ACCEPT Packet_0x22
#define PACKET_ACCOUNT_LOGIN_REQUEST Packet_0x80
#define PACKET_SERVER_LIST Packet_0xa8


#define Packet_0x80_length 62

class Client;

/*
* Methods required for packets:
* For incoming packets: 
*       const udword_t length();
*       virtual void receive(Socket *s) override;
* 
* For outgoing packets:
*       void set_data(...);
*/

class PacketIn_0x00 : public PacketIn {
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
    ~PacketIn_0x00();
private:
};

class PacketIn_0x02 : public PacketIn {
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
    ~PacketIn_0x02();
};

class PacketOut_0x21 : public PacketOut {
public:
    void set_data(t_ubyte seq, Client *client);
    PacketOut_0x21();
    ~PacketOut_0x21();
};

class PacketOut_0x22 : public PacketOut {
public:
    void set_data(t_ubyte seq, Client* client);
    PacketOut_0x22();
    ~PacketOut_0x22();
};

class PacketOut_0x55 : public PacketOut { //PacketLoginComplete (game character finished loading)
public:
    PacketOut_0x55();
    ~PacketOut_0x55() {}
};

class PacketIn_0x73 : public PacketIn { //PacketPing
public: // IO packet, has both read and write methods.
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
    PacketIn_0x73();
    ~PacketIn_0x73();
};

class PacketOut_0x73 : public PacketOut { //PacketPing
public:
    void set_data(t_ubyte response);
    PacketOut_0x73();
};

class PacketIn_0x80 : public PacketIn {  //LoginCredentials & ServerListRequest
    bool _is_valid_account = false;
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
    bool is_valid_account();
    ~PacketIn_0x80();
};

class PacketOut_0x82 : public PacketOut {  //PacketLoginResponse
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
    void set_data(ResponseCode code);
    PacketOut_0x82();
    ~PacketOut_0x82();
};

class PacketOut_0x8c : public PacketOut {
public:
    void set_data(Socket* s, word_t server_index);
    PacketOut_0x8c();
    ~PacketOut_0x8c();
};

class PacketIn_0x91 : public PacketIn {  //LoginCredentials & ServerListRequest
    std::string accName[30];
    std::string accPassword[30];
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
    ~PacketIn_0x91();
};

class PacketIn_0xa0 : public PacketIn {  //ServerSelect -> disconnects from login server, connects to game server and requests character's list and client's flags
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
    PacketIn_0xa0();
    ~PacketIn_0xa0();
};

class PacketOut_0xa8 : public PacketOut {  //ServerList
public:
    void set_data(Socket *s);
    PacketOut_0xa8();
    ~PacketOut_0xa8();
};

class PacketOut_0xa9 : public PacketOut { // Supported Features
public:
    void set_data(Client* client);
    PacketOut_0xa9();
    ~PacketOut_0xa9();
};

class PacketOut_0xb9 : public PacketOut { // Supported Features
public:
    void set_data(dword_t seq, Client* client);
    PacketOut_0xb9();
    ~PacketOut_0xb9();
};

class PacketIn_0xef : public PacketIn {
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
    PacketIn_0xef();
    virtual ~PacketIn_0xef();
    udword_t seed() { return _seed; }
private:
    udword_t _seed;
    udword_t _client_major_version;
    udword_t _client_minor_version;
    udword_t _client_revision_version;
    udword_t _client_prototype_version;
};


#endif //__TORUS_PACKETLIST_H
