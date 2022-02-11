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


//const udword_t Packet_0xb9::length() {
//    ADDTOCALLSTACK();
//    return 21;
//}

void Packet_0xb9::set_data(dword_t seq, Client* client)
{
    UNREFERENCED_PARAMETER(seq);
    UNREFERENCED_PARAMETER(client);
    set_packet_id(0xb9);
    write_dword(1);
}

Packet_0xb9::~Packet_0xb9()
{
}