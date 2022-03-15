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

void Packet_0x20::set_data(Char* character) {
    ADDTOCALLSTACK();
    write_udword(character->get_uid().get_uid());
    write_uword(uword_t(character->get_body()));
    write_byte(0);
    write_uword(character->get_color());
    write_byte(0); // TODO: Flags (0x01 = Frozen, 0x02 = Female, 0x04 = Flying, 0x08 = Yellow HealthBar, 0x10 = Ignore Mobiles, 0x40 = War Mode, 0x80 = Hidden)
    write_uword(character->get_pos().get_x());
    write_uword(character->get_pos().get_y());
    write_word(0);
    write_byte(t_byte(character->get_dir()));
    write_byte(character->get_pos().get_z());
}

}
}