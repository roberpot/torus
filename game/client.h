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

#ifndef __TORUS_GAME_CLIENT_H_
#define __TORUS_GAME_CLIENT_H_

#include <library/types.h>
#include <network/packets/packetlist.h>

class Account;
class Char;
class Socket;
class Packets::PacketOut;

class Client {
public:
    Client(Socket*s);
    ~Client();
    enum class InputCmd {
        ADD,
        GO,
        REMOVE,
        QTY        
    };
    static const std::wstring input_text_cmds[int(InputCmd::QTY)];
private:
    Socket*_socket;
public:
    Socket*get_socket();
    void send(PacketOut* packet);
private:
    t_ubyte _movement_sequence;  ///< Walking sequence.
    uqword_t _movement_last;    ///< Last walk packet received.
public:
    void event_walk(const t_ubyte &dir, const t_ubyte &seq, const udword_t &fast_walk_key);
    void add_response_code(Packets::Out::Packet_0x82::ResponseCode code);
    void event_disconnect();
    void event_character_login(const std::string &name, const dword_t &flags, const dword_t &login_count, const dword_t &slot, const dword_t &ip);
    void event_double_click(Uid& uid);
    void event_click(Uid& uid);
    void event_talk_ascii(const TalkMode& talkmode, const uword_t& color, const Font& font, const std::string& text);
    void event_talk_unicode(const TalkMode& talkmode, const uword_t& color, const Font& font, const std::wstring& text);

    bool event_input_cmd(const std::wstring& text);

    void add_character(Char* character);
    void update_move(Char* character);
    
private:
    Char *_char;
    Account *_account;
public:
    Char *get_char();
    void attatch(Account* acc);
};

#endif // __TORUS_GAME_CLIENT_H_