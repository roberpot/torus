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
#include "socket.h"
#include "../debug/info.h"
#include "../game/client.h"
#include "../game/char.h"


const t_udword Packet_0x21::length() {
    ADDTOCALLSTACK();
    return 8;
}

const t_byte * Packet_0x21::dumps() {
    ADDTOCALLSTACK();
    return 0;
}

void Packet_0x21::loads(const t_byte * b) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(b);
}

void Packet_0x21::loads(Socket * s) {
    ADDTOCALLSTACK(); 
    UNREFERENCED_PARAMETER(s);
}

void Packet_0x21::set_data(t_ubyte seq, Client * client) {
    ADDTOCALLSTACK();
    Char *chr = client->get_char();
    if (!chr) {
        return;
    }
    (*this) << 0x21;
    (*this) << seq;
    (*this) << chr->get_pos().x;
    (*this) << chr->get_pos().y;
    (*this) << (t_ubyte)chr->get_dir();
    (*this) << chr->get_pos().z;
}


Packet_0x21::~Packet_0x21() {
    ADDTOCALLSTACK();
}