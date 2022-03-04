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
#define PACKET_USE_REQUEST Packet_0x06  //Double click
#define PACKET_MOVEMENT_REQUEST Packet_0x02
#define PACKET_MOVEMENT_REJECT Packet_0x21
#define PACKET_MOVEMENT_ACCEPT Packet_0x22
#define PACKET_ACCOUNT_LOGIN_REQUEST Packet_0x80
#define PACKET_SERVER_LIST Packet_0xa8


#define Packet_0x80_length 62

#define PAPERDOLL_TEXT_LENGTH 60

class Char;
class Client;
class Uid;

/*
* Methods required for packets:
* For incoming packets: 
*       const udword_t length();
*       virtual void receive(Socket *s) override;
* 
* For outgoing packets:
*       void set_data(...);
*/


// Character creation
class PacketIn_0x00 : public PacketIn {
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
    ~PacketIn_0x00();
private:
};

// Movement request
class PACKET_MOVEMENT_REQUEST : public PacketIn {
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
};

// Double click
class PACKET_USE_REQUEST : public PacketIn {
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
};

// Login confirm
class PacketOut_0x1b : public PacketOut {
public:
    void set_data(Char* character);
    PacketOut_0x1b();
    ~PacketOut_0x1b();
};

// Mobile Status
class PacketOut_0x11 : public PacketOut {
public:
    void set_data(Char* character);
    PacketOut_0x11() : PacketOut(0x11, true) {};
};

// Movement rejected
class PACKET_MOVEMENT_REJECT : public PacketOut {
public:
    void set_data(t_ubyte seq, Char* character);
    PACKET_MOVEMENT_REJECT() : PacketOut(0x21) {};
};

// Movement accepted
class PACKET_MOVEMENT_ACCEPT : public PacketOut {
public:
    void set_data(const t_ubyte& seq, const udword_t& fast_walk_key);
    PACKET_MOVEMENT_ACCEPT() : PacketOut(0x22) {};
};

// Skills Update
class PacketOut_0x3a : public PacketOut {
public:
    void set_data(Char* character);
    PacketOut_0x3a() : PacketOut(0x3a, true) {};
};

// Character query
class PacketIn_0x34 : public PacketIn {
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
};

// PacketLoginComplete (game character finished loading)
class PacketOut_0x55 : public PacketOut {
public:
    PacketOut_0x55();
    ~PacketOut_0x55() {}
};

// Play character
class PacketIn_0x5d : public PacketIn {
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
    ~PacketIn_0x5d();
};

// Play Music
class PacketOut_0x6d : public PacketOut {
public:
    void set_data(t_ubyte id);
    PacketOut_0x6d();
};

// PacketPing In
class PacketIn_0x73 : public PacketIn {
public: // IO packet, has both read and write methods.
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
    PacketIn_0x73();
    ~PacketIn_0x73();
};

// PacketPing Out
class PacketOut_0x73 : public PacketOut {
public:
    void set_data(t_ubyte response);
    PacketOut_0x73();
};

// Send Char
class PacketOut_0x78 : public PacketOut {
public:
    void set_data(Char* character); //Char* character
    PacketOut_0x78();
};

// LoginCredentials & ServerListRequest
class PacketIn_0x80 : public PacketIn {
    bool _is_valid_account = false;
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
    bool is_valid_account();
    ~PacketIn_0x80();
};

// Paperdoll
class PacketOut_0x88 : public PacketOut {
public:
    void set_data(Char* character);
    PacketOut_0x88() : PacketOut(0x88) {};
};

// PacketLoginResponse
class PacketOut_0x82 : public PacketOut {
public:
    enum class ResponseCode { //Response codes, copied from SphereX
        Invalid = 0x00, // no account
        InUse   = 0x01, // already in use
        Blocked = 0x02, // client blocked
        BadPass = 0x03, // incorrect password
        Other   = 0x04, // other (e.g. timeout)

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

// Play server accepted
class PacketOut_0x8c : public PacketOut {
public:
    void set_data(Socket* s, word_t server_index);
    PacketOut_0x8c();
    ~PacketOut_0x8c();
};

// LoginCredentials & CharList Request
class PacketIn_0x91 : public PacketIn {
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

// ServerList
class PacketOut_0xa8 : public PacketOut {
public:
    void set_data(Socket *s);
    PacketOut_0xa8();
    ~PacketOut_0xa8();
};

// Characters List
class PacketOut_0xa9 : public PacketOut {
public:
    void set_data(Client* client);
    PacketOut_0xa9();
    ~PacketOut_0xa9();
};

// Supported Features
class PacketOut_0xb9 : public PacketOut {
public:
    void set_data(dword_t seq, Client* client);
    PacketOut_0xb9();
    ~PacketOut_0xb9();
};

// Client version (Entering World)
class PacketIn_0xbd : public PacketIn {
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
};

// Extended Command
class PacketIn_0xbf : public PacketIn {
    uword_t _length;
public:
    virtual const uword_t length() override;
    virtual void process(Socket* s) override;
    PacketIn_0xbf();
};

// Extended Command
class PacketOut_0xbf : public PacketOut {
public:
    void set_data();
    PacketOut_0xbf();
    ~PacketOut_0xbf();
};

// New Client Version
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
