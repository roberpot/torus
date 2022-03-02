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


const uword_t PacketIn_0xbd::length() {
    return 25;
}

void PacketIn_0xbd::process(Socket* s) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(s);
    udword_t client_major_version;
    udword_t client_minor_version;
    udword_t client_revision_version;
    udword_t client_prototype_version;
    udword_t unused;
    UNREFERENCED_PARAMETER(unused);

    *(this) >> client_major_version;
    *(this) >> client_minor_version;
    *(this) >> client_revision_version;
    *(this) >> client_prototype_version;

    //TODO: store the reported version.
}
