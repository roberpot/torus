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


const uword_t PACKET_MOVEMENT_REQUEST::length() {
    return 7;
}

void PACKET_MOVEMENT_REQUEST::process(Socket* s) {
    ADDTOCALLSTACK();
    t_byte dir = read_byte();
    t_ubyte sequence = read_ubyte();
    udword_t fast_walk_key = read_udword();
    s->get_client()->event_walk(dir, sequence, fast_walk_key);
}
