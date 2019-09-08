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


const t_udword Packet_0x82::length()
{
    ADDTOCALLSTACK();
    return 2;
}

void Packet_0x82::set_data(ResponseCode code)
{
    ADDTOCALLSTACK();
    set_packet_id(0x82);
    write_byte((t_byte)code);
}

Packet_0x82::~Packet_0x82()
{
}
