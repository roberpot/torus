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
#include <debug_support/info.h>
#include <game/uo_files/map_list.h>


namespace Packets
{
namespace Out
{

void Packet_0xbf::set_data()
{
    
}

void Packet_0xbf::sub_cmd_mapdiffs()
{
    write_word(0x18);
    write_dword(0); //Not sending mapdiffs (Only supporting newer clients ATM).
}

void Packet_0xbf::sub_cmd_map() {
    write_word(0x08);
    t_ubyte maps_count = maplist.get_map_count();

    write_ubyte(maps_count);

    for (t_ubyte i = 0; i <= maps_count; ++i) {
        write_dword(0);
        write_dword(0);
    }
}

}
}