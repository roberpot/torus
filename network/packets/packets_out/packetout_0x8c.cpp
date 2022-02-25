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
#include <core/config.h>
#include <debug_support/info.h>


//const udword_t Packet_0x8c::length() {
//    ADDTOCALLSTACK();
//    return 11;
//}

void PacketOut_0x8c::set_data(Socket* s, word_t server_index )
{
    UNREFERENCED_PARAMETER(s);
    server_index = 0; //FIXME: Use real index
    ServerInfo info = toruscfg._game_servers[server_index];
    std::vector<std::string> ip = split(info.ip, '.');

    write_ubyte(t_ubyte(atoi(ip[0].c_str())));
    write_ubyte(t_ubyte(atoi(ip[1].c_str())));
    write_ubyte(t_ubyte(atoi(ip[2].c_str())));
    write_ubyte(t_ubyte(atoi(ip[3].c_str())));
    write_uword(info.port);   //TODO: Send real PORT
    write_dword(0x7f000001);  //TODO: Add real calculation using zlib and account's name.
}

PacketOut_0x8c::PacketOut_0x8c() : PacketOut(0x8c)
{
}

PacketOut_0x8c::~PacketOut_0x8c() {
    ADDTOCALLSTACK();
}