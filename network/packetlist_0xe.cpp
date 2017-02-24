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

#include "packetlist.h"
#include "socket.h"
#include "../debug/info.h"


const uint32_t Packet_0xef::length() {
    ADDTOCALLSTACK();
    return 21;
}

const t_byte * Packet_0xef::dumps() {
    ADDTOCALLSTACK();
    return 0;
}

void Packet_0xef::loads(const t_byte * b) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(b);
}

#define N2L(C, LL) \
    LL = ((C&0xff000000))>>24 | ((C&0x00ff0000))>>8  | ((C&0x0000ff00))<<8 | ((C&0x000000ff)<<24)

void Packet_0xef::loads(Socket * s) {
    ADDTOCALLSTACK();
    *s >> _seed;
    N2L(_seed, _seed);
    *s >> _client_major_version;
    *s >> _client_minor_version;
    *s >> _client_revision_version;
    *s >> _client_prototype_version;
}

Packet_0xef::~Packet_0xef() {
    ADDTOCALLSTACK();
}