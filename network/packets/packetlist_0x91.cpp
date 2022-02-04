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


const udword_t Packet_0x91::length()
{
    return 62;
}

void Packet_0x91::loads(Socket* s)
{
    s->read_string(*s, *accName, 30);
    s->read_string(*s, *accPassword, 30);
    t_byte command;
    *s >> command;
    std::stringstream str;
    str << "Account connection request from ";
    str << accName->c_str();
    TORUSSHELLECHO(str.str());

    Packet_0xb9* packet = new Packet_0xb9();
    dword_t featureFlags = 1;
    packet->set_data(featureFlags, s->get_client());
    s->write_packet(packet);
    //s->set_closing();  // Must disconnect the client from login server?

    Packet_0xa9 *packetCharacterList = new Packet_0xa9();
    packetCharacterList->set_data(s->get_client());
    s->write_packet(packetCharacterList);
}

Packet_0x91::~Packet_0x91()
{
}
