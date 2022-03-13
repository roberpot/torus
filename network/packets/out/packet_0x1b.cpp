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

#include <game/char.h>
#include <game/uo_files/map_list.h>
#include <network/packets/packetlist.h>
#include <debug_support/info.h>

namespace Packets
{
namespace Out
{

void Packet_0x1b::set_data(Char* character)
{
    ADDTOCALLSTACK();
    write_dword(character->get_uid().get_uid());
    write_dword(0);              //Unused
    write_word(word_t(character->get_body()));
    write_word(character->get_pos().get_x());
    write_word(character->get_pos().get_y());
    write_word(character->get_pos().get_z());
    write_byte(t_byte(character->get_dir()));
    write_byte(0);              //Unused
    write_dword(0xFFFFFFFF);    //Unused
    write_word(0);              //Unused
    write_word(0);              //Unused
    Map* map = maplist.get_map(character->get_pos().get_map());
    //write_word(map->get_max_x());   //FIXME
    //write_word(map->get_max_y());
    write_word(7168);
    write_word(4096);
    write_word(0);              //Unused
    write_dword(0);             //Unused
}

}
}