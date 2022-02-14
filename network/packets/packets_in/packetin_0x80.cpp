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

#include <network/packets/packetlist.h>
#include <network/socket.h>
#include <debug_support/info.h>
#include <game/client.h>
#include <shell.h>

const udword_t PacketIn_0x80::length()
{
    return Packet_0x80_length;
}

void PacketIn_0x80::set_from_loginserver()
{
    _from_loginserver = true;
}

void PacketIn_0x80::process(Socket* s)
{
    if (s->get_connection_state() != ConnectionState::CONNECTIONSTATE_LOGIN) // Double casting to prevent Warning #C26812 (VS)
    {
        return;
    }
    (*this) >> accName;
    (*this) >> accPassword;
    t_byte command;
    *(this) >> command;
    std::stringstream str;
    str << "Account connection request from ";
    str << accName->c_str();
    TORUSSHELLECHO(str.str());

    _is_valid_account = true; // TODO: Add real account checks.

    if (s == nullptr) { //Sometimes happens at clients' closure.
        return;
    }
    if (!_is_valid_account)
    {
        //get_client()->add_response_code(Packet_0x82::ResponseCode::Success); // shouldn't be here?
    }
    if (_from_loginserver)
    {
        DEBUG_NOTICE("Received valid account identification, proceeding to send server information.");
        PacketOut_0xa8* packet = new PacketOut_0xa8();
        s->write(packet);
        s->set_connection_state(ConnectionState::CONNECTIONSTATE_SERVERLIST);
    }
}

bool PacketIn_0x80::is_valid_account()
{
    return _is_valid_account;
}

PacketIn_0x80::~PacketIn_0x80()
{
}
