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

#include "packet.h"
#include "packets/packetlist.h"

#include "../library/string.h"
#include "../core/errors.h"
#include "socket.h"



Packet * packet_factory(Socket & s) {
    ADDTOCALLSTACK();
    t_ubyte t = 0;
    s >> t;
    Packet * p = NULL;
    switch(t) {
        case 0xef: 
            p = new Packet_0xef();
            break;
        case 0x80:
            p = new Packet_0x80();
            break;
        default:

            TORUSSHELLECHO("Unknown packet code 0x" << std::hex << (t_udword)t << std::dec << ".");
            //THROW_ERROR(NetworkError, "Unknown packet code 0x" << std::hex << (t_udword)t << std::dec << ".");
    }
    if (p != nullptr)
    {
        p->loads(&s);
    }
    return p;
}

Packet * packet_factory(t_byte t) {
    switch(t) {
        default: THROW_ERROR(NetworkError, "Unknown packet code 0x" << std::hex << (t_udword)t << std::dec << ".");
    }
    //return 0;
}

Packet * packet_factory(const t_byte * buffer, t_udword len) {
    switch(buffer[0]) {
        default: THROW_ERROR(NetworkError, "Unknown packet [" << len << "] " << std::hex << print_hex_buffer(buffer, len) << std::dec << ".");
    }
    //return 0;
}


Packet::Packet() {
    buffer = 0;
    len = 0;
    pos = 0;
    count = 0;
}

const t_byte * Packet::dumps() {
    return buffer;
}

const t_udword Packet::length() {
    return len;
}

void Packet::send(Socket * s) {
    if (buffer) {
        s->write_packet(this);
    }
    else {
        THROW_ERROR(NetworkError, "Sending empty packet ");
    }
}

Packet::~Packet() {
    if (buffer) delete buffer;
}

void Packet::set_packet_id(t_ubyte id) {
    (*this) << id;
}

int Packet::get_count()
{
    return count;
}

void Packet::set_pos(int ipos)
{
    pos = ipos;
}

int Packet::get_pos()
{
    return pos;
}

void Packet::write_string(std::string& str, int len)
{
    memcpy((void*)buffer, (void*)str.c_str(), len);
    /*if (sizeof(T) == 4) {
        d = (((d & 0x000000ff) << 24) | ((d & 0x0000ff00) << 8) | ((d & 0x00ff0000) >> 8) | ((d & 0xff000000) >> 24));
    }*/
    len += str.length();
}

