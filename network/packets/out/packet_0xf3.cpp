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


namespace Packets
{
namespace Out
{

void Packet_0xf3::set_data(Char* character) {
    write_word(1);
    write_byte(1);
    write_udword(character->get_uid().get_uid());
    write_uword(uword_t(character->get_body()));
    write_byte(t_byte(character->get_dir()));
    write_word(1);
    write_word(1);
    write_uword(character->get_pos().get_x());
    write_uword(character->get_pos().get_y());
    write_byte(character->get_pos().get_z());
    write_byte(0);
    write_uword(character->get_color());
    write_byte(0x20);
    write_word(0);
}

void Packet_0xf3::set_data(Item* item) {
    write_word(1);
    write_byte(0);
    write_udword(item->get_uid().get_uid());
    write_uword(uword_t(item->get_id()));
    write_byte(t_byte(item->get_dir()));
    write_word(1);
    write_word(1);
    write_uword(item->get_pos().get_x() & 0x7FFF);
    write_uword(item->get_pos().get_y() & 0x3FFF);
    write_byte(item->get_pos().get_z());
    write_byte(1);  //Light
    write_uword(item->get_color());
    write_byte(0x20);
    write_word(0);
}

}
}