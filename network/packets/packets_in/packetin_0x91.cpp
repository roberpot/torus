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
#include <shell.h>


const uword_t PacketIn_0x91::length()
{
    return 62;
}

void PacketIn_0x91::process(Socket* s)
{
    int32_t junk;
    (*this) >> junk;
    (*this) >> accName;
    (*this) >> accPassword;
    t_byte command;
    *(this) >> command;
    std::stringstream str;
    str << "Account connection request from ";
    str << accName->c_str();
    TORUSSHELLECHO(str.str());

    PacketOut_0xb9* packet = new PacketOut_0xb9();
    dword_t featureFlags = 1;
    packet->set_data(featureFlags, s->get_client());
    packet->send(s);
    //s->set_closing();  // Must disconnect the client from login server?

    PacketOut_0xa9 *packetCharacterList = new PacketOut_0xa9();
    packetCharacterList->set_data(s->get_client());
    packetCharacterList->send(s);
}

PacketIn_0x91::~PacketIn_0x91()
{
}
