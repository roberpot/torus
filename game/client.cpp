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
#include <game/coord_point.h>
#include <game/item.h>
#include <game/uid.h>
#include <game/server.h>
#include <network/socket.h>
#include <network/packets/packetlist.h>


using namespace Packets::In;
using namespace Packets::Out;

const std::wstring Client::input_text_cmds[int(InputCmd::QTY)] = {
    L"ADD",
    L"GO",
    L"REMOVE"
};

Client::Client(Socket* s) :
    _socket(s),
    _movement_sequence(0),
    _movement_last(0),
    _target_type(TargetType::NONE),
    _target_action(TargetAction::NONE),
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
    packet->send(_socket);
}

void Client::event_walk(const t_ubyte& dir, const t_ubyte& seq, const udword_t& fast_walk_key) {
    ADDTOCALLSTACK();
    //_char MUST be valid.

    UNREFERENCED_PARAMETER(dir);

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
    ADDTOCALLSTACK();
    if (_char != nullptr) {
        _char = nullptr;
        //
    }
    add_response_code(Packets::Out::Packet_0x82::ResponseCode::Other);
    _socket->set_read_closed();
}

void Client::event_character_login(const std::string& name, const dword_t& flags, const dword_t& login_count, const dword_t& slot, const dword_t& ip)
{
    ADDTOCALLSTACK();
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

    LoginConfirm* packet_login_confirm = new LoginConfirm();
    packet_login_confirm->set_data(_char);
    send(packet_login_confirm);

    ExtendedCmdOut* packet_extended_cmd_mapdiffs = new ExtendedCmdOut();
    packet_extended_cmd_mapdiffs->sub_cmd_mapdiffs();
    send(packet_extended_cmd_mapdiffs);

    PlayMusic* packet_music = new PlayMusic();
    packet_music->set_data(0x09);   //TODO: Set region's musics and send real data.
    send(packet_music);

    ExtendedCmdOut* packet_extended_cmd_change_map = new ExtendedCmdOut();
    packet_extended_cmd_change_map->sub_cmd_map();
    send(packet_extended_cmd_change_map);

    add_character(_char);

    LoginDone* packet_login_done = new LoginDone();
    send(packet_login_done);

}

void Client::event_double_click(Uid& uid)
{
    ADDTOCALLSTACK();
    if (!uid.is_valid())
    {
        return;
    }
    if (uid.is_char())
    {
        Char* character = server.get_char(uid);        
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
    ADDTOCALLSTACK();
    std::string target_name;
    if (!uid.is_valid())
    {
        return;
    }
    Artifact* artifact = server.get_artifact(uid);
    target_name = artifact->get_name();
    AsciiMessageOut* packet_message = new AsciiMessageOut();
    uword_t hue = 0x0000;
    TalkMode talk_mode = TalkMode::SAY;
    Font font = Font::NORMAL;
    packet_message->set_data(target_name, artifact, hue, talk_mode, font);
    send(packet_message);
}

void Client::event_talk_ascii(const TalkMode& talkmode, const uword_t& color, const Font& font, const std::string& text) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(talkmode);
    UNREFERENCED_PARAMETER(color);
    UNREFERENCED_PARAMETER(font);
    if (text.find('.') == 0) {
        if (event_input_cmd(to_wstring(text)))             {
            return;
        }
    }
    TORUSSHELLECHO("Player " << _char->get_name() << " speaks \"" << text.c_str() << "\".");
}

void Client::event_talk_unicode(const TalkMode& talkmode, const uword_t& color, const Font& font, const std::wstring& text) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(talkmode);
    UNREFERENCED_PARAMETER(color);
    UNREFERENCED_PARAMETER(font);
    if (text.find('.') == 0) {
        if (event_input_cmd(text)) {
            return;
        }
    }
    TORUSSHELLECHOW(L"Player " << to_wstring(_char->get_name()) << L" speaks \"" << text.c_str() << "\".");
}

bool Client::event_input_cmd(const std::wstring& text) {
    ADDTOCALLSTACK();
    bool valid_cmd = true;
    std::wstring input_wstr(text);
    input_wstr.erase(0, 1);
    std::vector<std::wstring> entries = split(input_wstr, ' '); // split the keyword and params using the whitespace as separator.
    if (entries.size() > 0) {
        std::wstring input_cmd = clean(entries[0]); // First entry = keyword.
        entries.clear();    // reset the entries, to be set again.
        input_wstr.erase(0, input_cmd.size() + 1);  // remove the input cmd and the first whitespace from the string.
        entries = split(input_wstr, ',');   // split the text again, but using the comma as separator this time.
        InputCmd cmd = static_cast<InputCmd>(find_table(input_text_cmds, input_cmd));
        switch (cmd) {
            case InputCmd::ADD:
            {
                TargetType target_type = TargetType::OBJECT;
                TargetAction target_action = TargetAction::ADD;
                uword_t color = 0;
                ItemId id = ItemId::FORGE;
                add_target(target_type, target_action, id, color);
                break;
            }
            case InputCmd::GO: {
                if (entries.size() < 2) {
                    break;
                }
                if (entries.size() == 2) {
                    uword_t x = uword_t(std::stoi(clean(entries.at(0))));
                    uword_t y = uword_t(std::stoi(clean(entries.at(1))));
                    CoordPoint p;
                    if (!p.can_move_to_coord(x, y)) {
                        break;
                    }
                    CoordPoint old_p = _char->get_pos();
                    _char->set_pos(x, y);
                    update_move(_char, old_p);
                }
                else if (entries.size() == 3) {
                    uword_t x = uword_t(std::stoi(clean(entries.at(0))));
                    uword_t y = uword_t(std::stoi(clean(entries.at(1))));
                    t_byte z = t_byte(std::stoi(clean(entries.at(2))));
                    CoordPoint p;
                    if (!p.can_move_to_z(z)) {
                        break;
                    }
                    else if (!p.can_move_to_coord(x, y)) {
                        break;
                    }
                    CoordPoint old_p = _char->get_pos();
                    _char->set_pos(x, y, z);
                    update_move(_char, old_p);
                }
                else if (entries.size() == 4) {
                    uword_t x = uword_t(std::stoi(clean(entries.at(0))));
                    uword_t y = uword_t(std::stoi(clean(entries.at(1))));
                    t_byte z = t_byte(std::stoi(clean(entries.at(2))));
                    t_ubyte m = t_byte(std::stoi(clean(entries.at(3))));
                    CoordPoint p;
                    if (!p.can_move_to_map(m)) {
                        break;
                    }
                    else if (!p.can_move_to_z(z)) {
                        break;
                    }
                    else if (!p.can_move_to_coord(x, y)) {
                        break;
                    }
                    CoordPoint old_p = _char->get_pos();
                    _char->set_pos(x, y, z, m);
                    update_move(_char, old_p);
                }
                break;
            }
            case InputCmd::REMOVE:
                break;
            default:
                valid_cmd = false;        
                break;
        }
        if (valid_cmd) {
            TORUSSHELLECHOW(L"Player " << to_wstring(_char->get_name()) << L" inputs command \"" << text.c_str() << "\".");
        }
        else {
            // No need to output, an invalid command is 13handled as normal speak.
            //TORUSSHELLECHOW(L"Player " << to_wstring(_char->get_name()) << L" inputs invalid command \"" << text.c_str() << L"\".");
        }
    }
    return valid_cmd;
}

void Client::add_character(Char* character)
{
    ADDTOCALLSTACK();
    SendCharacter* packet_send_character = new SendCharacter();
    packet_send_character->set_data(_char);
    send(packet_send_character);
    //TODO: This is also used to update other chars on screen, using packet 0x77

    uword_t notoriety = 0x0;    // TODO: Notoriety
    t_ubyte status = 0x0;       // TODO:Poison

    MobileStatusBar* packet_status_bar = new MobileStatusBar();
    packet_status_bar->set_data(character->get_uid(), notoriety, status);
    send(packet_status_bar);

    UpdateCharacter* packet_mobile_update = new UpdateCharacter();
    packet_mobile_update->set_data(character);
    send(packet_mobile_update);

    MobileStatus* packet_mobile_status = new MobileStatus();
    packet_mobile_status->set_data(character);
    send(packet_mobile_status);
}

void Client::update_move(Char* character, const CoordPoint& old_p) {
    ADDTOCALLSTACK();
    //Myself        = Packet_0x20
    //Already seen  = Packet_0x77
    //Not seen yet  = Packet_0x78
    if (_char == character) {
        UpdateCharacter* packet_mobile_update = new UpdateCharacter();
        packet_mobile_update->set_data(character);
        send(packet_mobile_update);
    }
    else {
        //Calculate the distance between my character's position and the target's old position.
        uword_t dist = _char->get_pos().get_distance(old_p);
        if (dist > 18) {
            // The target was too far away, add it to my screen.
            add_character(character);
        }
        else {
            // The target was already in my screen, just move it.
            MoveCharacter* packet_move_character = new MoveCharacter();
            packet_move_character->set_data(character);
            send(packet_move_character);
        }
    }
    add_character(character);
}

void Client::add_item(Item* item) {
    ADDTOCALLSTACK();
    WorldObject* packet_world_object = new WorldObject();
    packet_world_object->set_data(item);
    send(packet_world_object);
}

void Client::get_target(Uid& uid, const t_byte& flags, Uid& target, const CoordPoint& pos, const ItemId& id) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(uid);
    UNREFERENCED_PARAMETER(flags);
    UNREFERENCED_PARAMETER(target);
    UNREFERENCED_PARAMETER(id);
    switch (_target_action) {
        case TargetAction::ADD:
        {
            if (!pos.is_valid_point()) {
                return;
            }
            Item *item = new Item();
            item->set_id(id);
            item->set_pos(pos);
            add_item(item);
            //TODO: World storage
        }
    }
}

void Client::add_target(const TargetType& target_type, const TargetAction& target_action, const ItemId& id, const uword_t& color) {
    ADDTOCALLSTACK();
    if (_target_type != TargetType::NONE) {
        // Already have a target ... cancel it?
    }
    if ((target_type == TargetType::NONE) ||
        (target_action == TargetAction::NONE)) {
        return;
    }
    AddTarget* packet_target = new AddTarget();
    t_byte flags = 0;
    packet_target->set_data(_char->get_uid(), target_type, flags, id, color);
    _target_type = target_type;
    _target_action = target_action;
    send(packet_target);
}

Char*  Client::get_char() {
    ADDTOCALLSTACK();
    return _char;
}

void Client::attatch(Account*  acc) {
    ADDTOCALLSTACK();
    _account = acc;
}

void Client::send_tooltip(const Uid& uid) {
    Tooltip tooltip;
    std::map<Uid, Tooltip>::iterator it = _tooltips.find(uid);
    bool request = false;
    if ( it == _tooltips.end()) {
        request = true;
    }
    else {
        if (tooltip.get_version() != server.get_artifact(uid)->get_tooltip().get_version()) {
            request = true;
        }
        else{
            tooltip = it->second;
        }
    }
    if (request) {
        tooltip = request_tooltip(uid);
        _tooltips[uid] = tooltip;
    }
    TooltipSend* packet_tooltip_send = new TooltipSend();
    packet_tooltip_send->set_data(tooltip);
    send(packet_tooltip_send);
}

Tooltip Client::request_tooltip(const Uid& uid) {
    Tooltip tooltip;
    Artifact *artifact = server.get_artifact(uid);
    if (artifact) {
        tooltip = artifact->get_tooltip();
        if (tooltip.has_personal_data()) {
            std::vector<Cliloc> clilocs = tooltip.get_clilocs();
            for (size_t i = 0; i < clilocs.size(); ++i) {
                Cliloc cliloc(clilocs[i]);
                if (cliloc.has_personal_data()) {
                    cliloc = artifact->get_cliloc_dynamic(cliloc.get_id(), _char);
                }
            }
        }
    }
    return tooltip;    
}

void Client::reset_tooltip(const Uid& uid) {
    _tooltips.erase(uid);
}

void Client::reset_tooltips() {
    _tooltips.clear();
}
