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
#include <game/char.h>


namespace Packets {
namespace Out {

void Packet_0x88::set_data(Char* character) {
  write_udword(character->get_uid().get_uid());
  write_string(character->get_name(), PAPERDOLL_TEXT_LENGTH);
  write_byte(0);  // TODO: Flags: 0x01 = WarMode, 0x02 = CanLift
}

}  // namespace Out
}  // namespace Packets
