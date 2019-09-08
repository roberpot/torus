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

#include "../library/types.h"
#include "../network/packets/packetlist.h"

class Socket;
class Char;

class Client {
public:
    Client(Socket *s);
    ~Client();
private:
    Socket *_socket;
public:
    Socket *get_socket();
    void send(Packet* packet);
private:
    t_ubyte _movement_sequence;  ///< Walking sequence.
    t_uqword _movement_last;    ///< Last walk packet received.
public:
    void event_walk(t_ubyte dir, t_ubyte seq);
    void add_response_code(Packet_0x82::ResponseCode code);
private:
    Char *_char;
public:
    Char *get_char();
    void attatch(Char *chr);
};

#endif // __TORUS_GAME_CLIENT_H_