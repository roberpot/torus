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
#include <game/client.h>
#include <debug_support/info.h>


namespace Packets
{
namespace In
{

const uword_t Packet_0x5d::length() {
    return 73;
}

void Packet_0x5d::process(Socket* s) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(s);
    skip(4);
    std::string character_name = read_string(CHARACTERS_STRING_LENGTH);
    skip(2);    
    dword_t flags = read_dword();
    skip(4);
    dword_t login_count = read_dword();
    skip(4);
    skip(4);
    skip(4);
    skip(4);
    dword_t slot = read_dword();
    dword_t ip = read_dword();
    s->get_client()->event_character_login(character_name, flags, login_count, slot, ip);
}

}
}