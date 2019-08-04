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

#include "packetlist.h"
#include "../socket.h"
#include "../../debug/info.h"


const t_udword Packet_0x55::length() {
    ADDTOCALLSTACK();
    return 21;
}

const t_byte * Packet_0x55::dumps() {
    ADDTOCALLSTACK();
    return buffer;
}

void Packet_0x55::loads(const t_byte * b) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(b);
}

void Packet_0x55::loads(Socket * s) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(s);
}

void Packet_0x55::set_data(t_ubyte seq, Client* client)
{
}

Packet_0x55::Packet_0x55() {
    ADDTOCALLSTACK();
    buffer = new t_byte[1];
    set_packet_id(0x55);
}

Packet_0x55::~Packet_0x55() {
    ADDTOCALLSTACK();
}