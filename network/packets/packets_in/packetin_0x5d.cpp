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
#include <game/client.h>
#include <debug_support/info.h>


const uword_t PacketIn_0x5d::length() {
    return 73;
}

void PacketIn_0x5d::process(Socket* s) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(s);
    dword_t junk1;
    std::string character_name;
    word_t junk2;
    dword_t flags;
    dword_t junk3;
    dword_t login_count;
    dword_t junk4;
    dword_t junk5;
    dword_t junk6;
    dword_t junk7;
    dword_t slot;
    dword_t ip;

    *(this) >> junk1;
    read_string(character_name, CHARACTERS_STRING_LENGTH);
    *(this) >> junk2;
    *(this) >> flags;
    *(this) >> junk3;
    *(this) >> login_count;
    *(this) >> junk4;
    *(this) >> junk5;
    *(this) >> junk6;
    *(this) >> junk7;
    *(this) >> slot;
    *(this) >> ip;
    s->get_client()->event_character_login(character_name, flags, login_count, slot, ip);
}