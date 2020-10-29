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
#include "../../debug_support/info.h"


const udword_t Packet_0x8c::length() {
    ADDTOCALLSTACK();
    return 10;
}

void Packet_0x8c::set_data(Socket* s)
{
    udword_t ip = 16777343;  //127.0.0.1    //TODO: send real IP
    write_byte(ip & 0xFF);
    write_byte((ip >> 8) & 0xFF);
    write_byte((ip >> 16) & 0xFF);
    write_byte((ip >> 24) & 0xFF);
    write_word(2593);   //TODO: Send real PORT
    write_dword(1456773342);  //TODO: Add real calculation using zlib and account's name.
}

Packet_0x8c::~Packet_0x8c() {
    ADDTOCALLSTACK();
}