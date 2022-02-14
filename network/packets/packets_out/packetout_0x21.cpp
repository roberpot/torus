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
#include <network/packets/packetlist.h>
#include <network/socket.h>
#include <debug_support/info.h>
#include <game/client.h>
#include <game/char.h>


//const udword_t Packet_0x21::length() {
//    ADDTOCALLSTACK();
//    return 8;
//}

void PacketOut_0x21::set_data(t_ubyte seq, Client * client) {
    ADDTOCALLSTACK();
    Char *chr = client->get_char();
    if (!chr) {
        return;
    }
    set_packet_id(0x21);
    (*this) << (seq);
    (*this) << chr->get_pos().x;
    (*this) << chr->get_pos().y;
    (*this) << (t_ubyte)chr->get_dir();
    (*this) << t_ubyte(chr->get_pos().z);
}

PacketOut_0x21::PacketOut_0x21() : PacketOut(0x21)
{
}


PacketOut_0x21::~PacketOut_0x21() {
    ADDTOCALLSTACK();
}