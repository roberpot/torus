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
#include <debug_support/info.h>
#include <game/client.h>
#include <game/char.h>
#include <network/packets/packetlist.h>
#include <network/socket.h>


namespace Packets
{
namespace Out
{

void Packet_0x22::set_data(const t_ubyte& seq, const udword_t& fast_walk_key) {
    ADDTOCALLSTACK();
    write_ubyte(seq);
    write_ubyte(7); // View color (blue, red, yellow ...)
    write_udword(fast_walk_key);
}

}
}