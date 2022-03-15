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
#include <game/uid.h>

namespace Packets
{
namespace Out
{


void Packet_0x17::set_data(Uid& uid, const uword_t& color, const t_ubyte& flag)
{
    write_udword(uid.get_uid());
    write_word(0x01);
    write_uword(color); //Green Bar
    write_ubyte(flag);  //is_poisoned?
    write_uword(color); //Yellow bar
    write_ubyte(flag);  // is_paralyzed?
}

}
}