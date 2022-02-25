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


const uword_t PacketIn_0xef::length() {
    return 21;
}

void PacketIn_0xef::process(Socket* s) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(s);
    *(this) >> _seed; 
    *(this) >> _client_major_version;
    *(this) >> _client_minor_version;
    *(this) >> _client_revision_version;
    *(this) >> _client_prototype_version;

    s->set_seed(_seed);
}

PacketIn_0xef::PacketIn_0xef()
{
    _seed = 0;
    _client_major_version = 0;
    _client_minor_version = 0;
    _client_revision_version = 0;
    _client_prototype_version = 0;
}

PacketIn_0xef::~PacketIn_0xef() {
}