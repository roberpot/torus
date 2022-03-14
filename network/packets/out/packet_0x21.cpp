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

#include <library/system_headers.h>
#include <network/packets/packetlist.h>
#include <network/socket.h>
#include <debug_support/info.h>
#include <game/client.h>
#include <game/char.h>


namespace Packets
{
namespace Out
{

void Packet_0x21::set_data(t_ubyte seq, Char* character) {
    ADDTOCALLSTACK();
    write_ubyte(seq);
    write_word(character->get_pos().get_x());
    write_word(character->get_pos().get_y());
    write_byte(t_byte(character->get_dir()));
    write_ubyte(character->get_pos().get_z());
}

}
}