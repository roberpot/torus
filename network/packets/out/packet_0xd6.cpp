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
#include <game/char.h>
#include <game/item.h>
#include <game/uo_files/map_list.h>
#include <game/clients/tooltip.h>


namespace Packets
{
namespace Out
{

void Packet_0xd6::set_data(const Uid& uid, const Tooltip& tooltip) {
    write_word(1);
    write_udword(uid.get_uid());
    write_word(0);
    write_dword(tooltip.get_version());
    auto clilocs = tooltip.get_clilocs();
    for (size_t i = 0; i < clilocs.size(); ++i){
        Cliloc cliloc = clilocs[i];
        write_dword(cliloc.get_id());
        std::wstring str = cliloc.get_args();
        write_uword(uword_t(str.size()));
        write_wstring(str);        
    }
}

}
}