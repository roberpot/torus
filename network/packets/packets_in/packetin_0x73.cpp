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


const uword_t PacketIn_0x73::length() {
    ADDTOCALLSTACK();
    return 2;
}

void PacketIn_0x73::process(Socket* s) {
    ADDTOCALLSTACK();
    PacketOut_0x73 *response = new PacketOut_0x73();
    response->set_data(1);
    response->send(s);
}

PacketIn_0x73::PacketIn_0x73()
{
}

PacketIn_0x73::~PacketIn_0x73() {
    ADDTOCALLSTACK();
}