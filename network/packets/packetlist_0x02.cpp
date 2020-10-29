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


const udword_t Packet_0x02::length() {
    ADDTOCALLSTACK();
    return 7;
}

void Packet_0x02::loads(const t_byte * b) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(b);
}

void Packet_0x02::loads(Socket * s) {
    ADDTOCALLSTACK();
    t_byte dir = 0;
    t_byte sequence = 0;
    *s >> dir;
    *s >> sequence;
    s->get_client()->event_walk(dir, sequence);
}

Packet_0x02::~Packet_0x02() {
    ADDTOCALLSTACK();
}