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
#include <game/accounts_manager.h>
#include <game/char.h>
#include <game/client.h>
#include <game/uid.h>
#include <game/server.h>
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

void Client::event_walk(const t_ubyte& dir, const t_ubyte& seq, const udword_t& fast_walk_key) {
    ADDTOCALLSTACK();
    //_char MUST be valid.



    PACKET_MOVEMENT_ACCEPT* packet_movement_accept = new PACKET_MOVEMENT_ACCEPT();
    packet_movement_accept->set_data(seq, fast_walk_key);
    packet_movement_accept->send(_socket);
    return; //TODO: WalkChecks
    if (Dir(dir) > Dir::DIR_QTY) {
        PACKET_MOVEMENT_REJECT* packet_movement_reject = new PACKET_MOVEMENT_REJECT();
        packet_movement_reject->set_data(seq, _char);
        packet_movement_reject->send(_socket);
        return;
    }
    else if (get_char()->get_pos().can_move_to_coord(1, 1))
    {
        _movement_sequence++;
        PACKET_MOVEMENT_ACCEPT* packet_movement_accept = new PACKET_MOVEMENT_ACCEPT();
        packet_movement_accept->set_data(seq, fast_walk_key);
        packet_movement_accept->send(_socket);
        
    }
    else {
        PACKET_MOVEMENT_REJECT* packet_movement_reject = new PACKET_MOVEMENT_REJECT();
        packet_movement_reject->set_data(seq, _char);
        packet_movement_reject->send(_socket);
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
    add_response_code(PacketOut_0x82::ResponseCode::Other);
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

    _char = _account->get_char(slot);

    PacketOut_0x1b *packet_login_confirm = new PacketOut_0x1b();
    packet_login_confirm->set_data(_char);
    packet_login_confirm->send(_socket);

    PacketOut_0x78* packet_send_character = new PacketOut_0x78();
    packet_send_character->set_data(_char);
    packet_send_character->send(_socket);

    PacketOut_0x55 *packet_login_confirmed = new PacketOut_0x55();
    packet_login_confirmed->send(_socket);

}

void Client::event_double_click(Uid& uid)
{
    if (uid.is_char())
    {
        Char *character = server.get_char(uid);        
        /*if ()//TODO: Is pet & rideable? -> mount
        {

        }
        else*/
        {
            PacketOut_0x88* packet_paperdoll = new PacketOut_0x88();
            packet_paperdoll->send(_socket);
        }
        
    }
}

void Client::event_click(Uid& uid)
{

}

Char * Client::get_char() {
    ADDTOCALLSTACK();
    return _char;
}

void Client::attatch(Account* acc) {
    ADDTOCALLSTACK();
    _account = acc;
}
