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


const udword_t Packet_0xa0::length() {
    ADDTOCALLSTACK();
    return 3;
}

void Packet_0xa0::loads(Socket * s) {
    ADDTOCALLSTACK();
    word_t serverIndex;
    *s >> serverIndex;

    //TODO: Receive and handle disconnect from loginserver, send character list and send client' flags.
}

Packet_0xa0::Packet_0xa0()
{
}

Packet_0xa0::~Packet_0xa0() {
    ADDTOCALLSTACK();
}