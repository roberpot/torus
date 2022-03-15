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

#include <game/enums.h>
#include <network/packetin.h>
#include <network/packetout.h>

#include <string>

class Artifact;
class Char;
class Client;
class Item;
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

namespace Packets
{

#define PAPERDOLL_TEXT_LENGTH 60
#define TEXT_LENGTH 30

namespace In
{
    // Character creation
    class Packet_0x00 : public PacketIn {
    public:
        virtual const uword_t length() override;
        virtual void process(Socket* s) override;
        ~Packet_0x00();
    private:
    };

    // Movement request 
    class Packet_0x02 : public PacketIn {
    public:
        virtual const uword_t length() override;
        virtual void process(Socket* s) override;
    };

    // Double click
    class Packet_0x06 : public PacketIn {
    public:
        virtual const uword_t length() override;
        virtual void process(Socket* s) override;
    };

    // Single click
    class Packet_0x09 : public PacketIn {
    public:
        virtual const uword_t length() override;
        virtual void process(Socket* s) override;
    };

    // Character query
    class Packet_0x34 : public PacketIn {
    public:
        virtual const uword_t length() override;
        virtual void process(Socket* s) override;
    };

    // Play character
    class Packet_0x5d : public PacketIn {
    public:
        virtual const uword_t length() override;
        virtual void process(Socket* s) override;
    };

    // PacketPing In
    class Packet_0x73 : public PacketIn {
    public: // IO packet, has both read and write methods.
        virtual const uword_t length() override;
        virtual void process(Socket* s) override;
    };

    // LoginCredentials & ServerListRequest
    class Packet_0x80 : public PacketIn {
        bool _is_valid_account = false;
    public:
        virtual const uword_t length() override;
        virtual void process(Socket* s) override;
        bool is_valid_account();
    };

    // LoginCredentials & CharList Request
    class Packet_0x91 : public PacketIn {
        std::string accName[30];
        std::string accPassword[30];
    public:
        virtual const uword_t length() override;
        virtual void process(Socket* s) override;
    };

    class Packet_0xa0 : public PacketIn {  //ServerSelect -> disconnects from login server, connects to game server and requests character's list and client's flags
    public:
        virtual const uword_t length() override;
        virtual void process(Socket* s) override;
    };

    // Client version (Entering World)
    class Packet_0xbd : public PacketIn {
    public:
        virtual const uword_t length() override;
        virtual void process(Socket* s) override;
        Packet_0xbd() : PacketIn(true) {};
    };

    // Extended Command
    class Packet_0xbf : public PacketIn {
        uword_t _length;
    public:
        virtual const uword_t length() override;
        virtual void process(Socket* s) override;
        Packet_0xbf() : PacketIn(true) {};
    };

    // New Client Version
    class Packet_0xef : public PacketIn {
    public:
        virtual const uword_t length() override;
        virtual void process(Socket* s) override;
        Packet_0xef();
        udword_t seed() { return _seed; }
    private:
        udword_t _seed;
        udword_t _client_major_version;
        udword_t _client_minor_version;
        udword_t _client_revision_version;
        udword_t _client_prototype_version;
    };

    using CreateCharacter   = Packet_0x00;
    using MovementRequest   = Packet_0x02;
    using UseRequest        = Packet_0x06;
    using ClickRequest      = Packet_0x06;
    using QueryCharacter    = Packet_0x34;
    using PlayCharacter     = Packet_0x5d;
    using Ping              = Packet_0x73;
    using LoginConnect      = Packet_0x80;
    using ServerConnect     = Packet_0x91;
    using ServerSelect      = Packet_0xa0;
    using ReportCliver      = Packet_0xbd;
    using ExtendedCmdIn     = Packet_0xbf;
    using ReportCliverNew   = Packet_0xef;


}

namespace Out
{
    // Login confirm
    class Packet_0x1b : public PacketOut {
    public:
        void set_data(Char* character);
        Packet_0x1b() : PacketOut(0x1b) {};
    };
    // Ascii Message
    class Packet_0x1c : public PacketOut {
    public:
        void set_data(const std::string& text, Artifact* target, const word_t& hue,
                      const TalkMode& talk_mode, const Font& font);
        Packet_0x1c() : PacketOut(0x1c, true) {};
    };

    // Mobile Status
    class Packet_0x11 : public PacketOut {
    public:
        void set_data(Char* character);
        Packet_0x11() : PacketOut(0x11, true) {};
    };

    // Mobile Status Bar
    class Packet_0x17 : public PacketOut {
    public:
        void set_data(Uid& uid, const uword_t& color, const t_ubyte& flag);
        Packet_0x17() : PacketOut(0x17, true) {};
    };

    // Movement rejected
    class Packet_0x21 : public PacketOut {
    public:
        void set_data(t_ubyte seq, Char* character);
        Packet_0x21() : PacketOut(0x21) {};
    };

    // Movement accepted
    class Packet_0x22 : public PacketOut {
    public:
        void set_data(const t_ubyte& seq, const udword_t& fast_walk_key);
        Packet_0x22() : PacketOut(0x22) {};
    };

    // Skills Update
    class Packet_0x3a : public PacketOut {
    public:
        void set_data(Char* character);
        Packet_0x3a() : PacketOut(0x3a, true) {};
    };

    // PacketLoginComplete (game character finished loading)
    class Packet_0x55 : public PacketOut {
    public:
        Packet_0x55() : PacketOut(0x55) {};
    };

    // Play Music
    class Packet_0x6d : public PacketOut {
    public:
        void set_data(t_ubyte id);
        Packet_0x6d() : PacketOut(0x6d) {};
    };

    // PacketPing Out
    class Packet_0x73 : public PacketOut {
    public:
        void set_data(t_ubyte response);
        Packet_0x73() : PacketOut(0x73) {};
    };

    // Send Char
    class Packet_0x78 : public PacketOut {
    public:
        void set_data(Char* character); //Char* character
        Packet_0x78() : PacketOut(0x78, true) {};
    };

    // PacketLoginResponse
    class Packet_0x82 : public PacketOut {
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
        Packet_0x82() : PacketOut(0x82) {};
    };

    // Paperdoll
    class Packet_0x88 : public PacketOut {
    public:
        void set_data(Char* character);
        Packet_0x88() : PacketOut(0x88) {};
    };

    // Play server accepted
    class Packet_0x8c : public PacketOut {
    public:
        void set_data(Socket* s, word_t server_index);
        Packet_0x8c() : PacketOut(0x8c) {};
    };

    // ServerList
    class Packet_0xa8 : public PacketOut {
    public:
        void set_data(Socket *s);
        Packet_0xa8() : PacketOut(0xa8, true) {};
    };

    // Characters List
    class Packet_0xa9 : public PacketOut {
    public:
        void set_data(Client* client);
        Packet_0xa9() : PacketOut(0xa9, true) {};
    };

    // Supported Features
    class Packet_0xb9 : public PacketOut {
    public:
        void set_data(dword_t seq, Client* client);
        Packet_0xb9() : PacketOut(0xb9) {};
    };

    // Extended Command
    class Packet_0xbf : public PacketOut {
    public:
        void set_data();
        void sub_cmd_mapdiffs();
        void sub_cmd_map(const t_ubyte& map);
        Packet_0xbf() : PacketOut(0xBF, true) {};
    };

    using MobileStatus      = Packet_0x11;
    using MobileStatusBar   = Packet_0x17;
    using LoginConfirm      = Packet_0x1b;
    using AsciiMessage      = Packet_0x1c;
    using MovementReject    = Packet_0x21;
    using MovementAccept    = Packet_0x22;
    using SkillsUpdate      = Packet_0x3a;
    using LoginDone         = Packet_0x55;
    using PlayMusic         = Packet_0x6d;
    using PingResponse      = Packet_0x73;
    using SendCharacter     = Packet_0x78;
    using LoginResponse     = Packet_0x82;
    using SendPaperdoll     = Packet_0x88;
    using ServerAccept      = Packet_0x8c;
    using ServerList        = Packet_0xa8;
    using CharList          = Packet_0xa9;
    using SupportedFeatures = Packet_0xb9;
    using ExtendedCmdOut    = Packet_0xbf;
}
}


#endif //__TORUS_PACKETLIST_H
