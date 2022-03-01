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

#include <game/accounts_manager.h>
#include <game/account.h>


const uword_t PacketIn_0x34::length() {
    return 10;
}

void PacketIn_0x34::process(Socket* s) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(s);
    
    dword_t junk;
    t_byte type;
    udword_t uid;

    (*this) >> junk;
    (*this) >> type;
    (*this) >> uid;
    Account * acc = torusacc.get_account("test");
    Char *character = acc->get_char(0);
    PacketOut_0x78* packet_send_character = new PacketOut_0x78();
    packet_send_character->set_data(character);
    packet_send_character->send(s);
}
