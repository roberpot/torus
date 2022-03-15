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


using namespace Packets::In;
using namespace Packets::Out;

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

    MovementAccept* packet_movement_accept = new MovementAccept();
    packet_movement_accept->set_data(seq, fast_walk_key);
    send(packet_movement_accept);
    return; //TODO: WalkChecks
    /*if (Dir(dir) > Dir::DIR_QTY) {
        MovementReject* packet_movement_reject = new MovementReject();
        packet_movement_reject->set_data(seq, _char);
        send(packet_movement_reject);
        return;
    }
    else if (get_char()->get_pos().can_move_to_coord(1, 1))
    {
        _movement_sequence++;
        MovementAccept* packet_movement_accept = new MovementAccept();
        packet_movement_accept->set_data(seq, fast_walk_key);
        send(packet_movement_accept);
        
    }
    else {
        MovementReject* packet_movement_reject = new MovementReject();
        packet_movement_reject->set_data(seq, _char);
        send(packet_movement_reject);
    }*/
}

void Client::add_response_code(Packets::Out::Packet_0x82::ResponseCode code)
{
    ADDTOCALLSTACK();
    LoginResponse* packet_login_response = new LoginResponse;
    packet_login_response->set_data(code);
    send(packet_login_response);
}

void Client::event_disconnect()
{
    if (_char != nullptr) {
        _char = nullptr;
        //
    }
    add_response_code(Packets::Out::Packet_0x82::ResponseCode::Other);
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

    LoginConfirm *packet_login_confirm = new LoginConfirm();
    packet_login_confirm->set_data(_char);
    send(packet_login_confirm);

    ExtendedCmdOut* packet_extended_cmd_mapdiffs = new ExtendedCmdOut();
    packet_extended_cmd_mapdiffs->sub_cmd_mapdiffs();
    send(packet_extended_cmd_mapdiffs);

    PlayMusic *packet_music = new PlayMusic();
    packet_music->set_data(0x09);   //TODO: Set region's musics and send real data.
    send(packet_music);

    ExtendedCmdOut* packet_extended_cmd_change_map = new ExtendedCmdOut();
    packet_extended_cmd_change_map->sub_cmd_map(_char->get_pos().get_map());
    send(packet_extended_cmd_change_map);

    add_character(_char);

    LoginDone *packet_login_done = new LoginDone();
    send(packet_login_done);

}

void Client::event_double_click(Uid& uid)
{
    if (!uid.is_valid())
    {
        return;
    }
    if (uid.is_char())
    {
        Char *character = server.get_char(uid);        
        /*if ()//TODO: Is pet & rideable? -> mount
        {

        }
        else*/
        {
            SendPaperdoll* packet_paperdoll = new SendPaperdoll();
            packet_paperdoll->set_data(character);
            send(packet_paperdoll);
        }
        
    }
}

void Client::event_click(Uid& uid)
{
    std::string target_name;
    if (!uid.is_valid())
    {
        return;
    }
    Artifact *artifact = server.get_artifact(uid);
    target_name = artifact->get_name();
    AsciiMessage* packet_message = new AsciiMessage();
    uword_t hue = 0x0000;
    TalkMode talk_mode = TalkMode::SAY;
    Font font = Font::NORMAL;
    packet_message->set_data(target_name, artifact, hue, talk_mode, font);
    send(packet_message);
}

void Client::add_character(Char* character)
{
    SendCharacter* packet_send_character = new SendCharacter();
    packet_send_character->set_data(_char);
    send(packet_send_character);
    //TODO: This is also used to update other chars on screen, using packet 0x77

    uword_t notoriety = 0x0;    // TODO: Notoriety
    t_ubyte status = 0x0;       // TODO:Poison
    MobileStatusBar *packet_status_bar = new MobileStatusBar();
    packet_status_bar->set_data(character->get_uid(), notoriety, status);
    send(packet_status_bar);


}

Char * Client::get_char() {
    ADDTOCALLSTACK();
    return _char;
}

void Client::attatch(Account* acc) {
    ADDTOCALLSTACK();
    _account = acc;
}
