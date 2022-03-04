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
#include <game/server.h>
#include <game/uid.h>


PacketOut_0x78::PacketOut_0x78() : PacketOut(0x78, true) {
    ADDTOCALLSTACK();
}

void PacketOut_0x78::set_data(Char* character)//(Char* character)
{
    //Char *character = server.get_char(uid);
    if (character == nullptr)
    {
        //err
        return;
    }
    write_udword(character->get_uid().get_uid());
    write_uword(character->get_pos().get_x());
    write_uword(character->get_pos().get_y());
    write_byte(character->get_pos().get_z());
    write_byte(t_byte(character->get_dir()));
    write_uword(character->get_color());
    write_byte(0);      //TODO: Calculate real flags for this packet.
    write_byte(1);      //TODO: Notoriety
    /*
    * loop
    *   write_udword(item->get_uid());
    *   write_uword(item->get_id());
    *   write_byte(item->get_layer());
    *   write_uword(item->get_color());
    * endloop
    */
    write_dword(0);
}
