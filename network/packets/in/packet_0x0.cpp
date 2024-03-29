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


namespace Packets
{
namespace In
{

const uword_t Packet_0x00::length() {
    ADDTOCALLSTACK();
    return 7;
}

void Packet_0x00::process(Socket* s) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(s);
}

Packet_0x00::~Packet_0x00() {
    ADDTOCALLSTACK();
}

}
}