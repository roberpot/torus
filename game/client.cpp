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

#include <library/system_headers.h>
#include <debug_support/callstack.h>
#include <game/account.h>
#include <game/char.h>
#include <game/client.h>
#include <network/socket.h>
#include <network/packets/packetlist.h>

Client::Client(Socket * s) :
    _socket(s),
    _movement_sequence(0),
    _movement_last(0),
    _char(nullptr),
    _account(nullptr)
{
    ADDTOCALLSTACK();
}

Client::~Client() {
    ADDTOCALLSTACK();
}

Socket* Client::get_socket() {
    return _socket;
}

void Client::send(PacketOut* packet)
{
    ADDTOCALLSTACK();
    _socket->write(packet);
}

void Client::event_walk(t_ubyte dir, t_ubyte seq) {
    ADDTOCALLSTACK();
    if (Dir(dir) > Dir::DIR_QTY) {
        PacketOut_0x21 *rej = new PacketOut_0x21();
        rej->set_data(seq, this);
    }
    if (get_char()->get_pos().can_move_to_coord(1, 1)) {
        _movement_sequence++;

    }
    else {
        PacketOut_0x21*rej = new PacketOut_0x21();
        rej->set_data(seq, this);
    }
}

void Client::add_response_code(PacketOut_0x82::ResponseCode code)
{
    ADDTOCALLSTACK();
    PacketOut_0x82* packet = new PacketOut_0x82();
    packet->set_data(code);
    send(packet);
}

void Client::event_disconnect()
{
    if (_char != nullptr) {
        _char = nullptr;
        //
    }
    _socket->set_read_closed();
}

void Client::event_character_login(const std::string& name, const dword_t& flags, const dword_t& login_count, const dword_t& slot, const dword_t& ip)
{
    UNREFERENCED_PARAMETER(name);       //TODO
    UNREFERENCED_PARAMETER(flags);      //TODO
    UNREFERENCED_PARAMETER(login_count);//TODO
    UNREFERENCED_PARAMETER(ip);         //TODO
    if (_account->get_char_count() < slot)
    {
        //wrong character id
        event_disconnect();
        return;
    }

    Char* character = _account->get_char(slot);

    PacketOut_0x1b *packet_login_confirm = new PacketOut_0x1b();
    packet_login_confirm->set_data(character);
    packet_login_confirm->send(_socket);
}

Char * Client::get_char() {
    ADDTOCALLSTACK();
    return _char;
}

void Client::attatch(Account* acc) {
    ADDTOCALLSTACK();
    _account = acc;
}
