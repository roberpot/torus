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
#include <core/torus.h>
#include <game/client.h>


const udword_t Packet_0x73::length() {
    ADDTOCALLSTACK();
    return 1;
}

void Packet_0x73::receive(Socket * s) {
    ADDTOCALLSTACK();
    s->write_packet(this);
    Packet_0x73 *response = new Packet_0x73();
    response->set_data(1, s);
    s->write_packet(response);
}

void Packet_0x73::set_data(t_ubyte ping, Socket *)
{
    set_packet_id(0x73);
    write_byte(ping);
}

Packet_0x73::Packet_0x73()
{
}

Packet_0x73::~Packet_0x73() {
    ADDTOCALLSTACK();
}