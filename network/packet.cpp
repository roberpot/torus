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

#include "../library/string.h"
#include "../core/errors.h"
#include "socket.h"



Packet * packet_factory(Socket & s) {
    t_byte t = 0;
    s >> t;
    switch(t) {
        default: THROW_ERROR(NetworkError, "Unknown packet code 0x" << std::hex << (t_udword)t << std::dec << ".");
    }
    return 0;
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

const t_byte * Packet::dumps() {
    return buffer;
}

const t_udword Packet::length() {
    return len;
}

Packet::~Packet() {
    delete buffer;
}