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
#include "../socket.h"
#include "../../debug/info.h"


const t_udword Packet_0x80::length()
{
    return 62;
}

const t_byte* Packet_0x80::dumps()
{
    ADDTOCALLSTACK();
    return 0;
}

void Packet_0x80::loads(const t_byte* b)
{
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(b);
}

void Packet_0x80::loads(Socket* s)
{
    s->read_string(*s, *accName, 30);
    s->read_string(*s, *accPassword, 30);
    byte command;
    *s >> command;
    std::stringstream str;
    str << "Account connection request from ";
    str << accName->c_str();
    TORUSSHELLECHO(str.str());

    Packet_0xa8* packet = new Packet_0xa8();
    s->write_packet(packet);
    delete packet;
}

Packet_0x80::~Packet_0x80()
{
}
