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
#include <core/torus.h>
#include <game/client.h>
#include <game/uid.h>
#include <game/coord_point.h>


namespace Packets
{
namespace In
{

const uword_t Packet_0x6c::length() {
    return 19;
}

void Packet_0x6c::process(Socket*s) {
    ADDTOCALLSTACK();
    skip(1);//type
    Uid uid = read_udword();
    t_byte flags = read_byte();
    Uid target = read_udword();
    uword_t x = read_uword();
    uword_t y = read_uword();
    skip(1); // z received in a dword, skip first byte.
    t_byte z = read_byte();
    CoordPoint p;
    p.set_point(x, y, z);
    ItemId id = ItemId(read_uword());
    s->get_client()->get_target(uid, flags, target, p, id);
}

}
}