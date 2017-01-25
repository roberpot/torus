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
#include "packet.h"

Packet * packet_factory(BYTE t) {
    switch(t) {
        default: THROW_ERROR(NetworkError, "Unknown packet code 0x" << std::hex << (unsigned int)t << std::dec << ".");
    }
    return NULL;
}

Packet * packet_factory(const char * buffer, unsigned int len) {
    switch(buffer[0]) {
        default: THROW_ERROR(NetworkError, "Unknown packet [" << len << "] " << std::hex << print_hex_buffer(buffer, len) << std::dec << ".");
    }
    return NULL;
}

const char * Packet::dumps() {
    return buffer;
}

const unsigned int Packet::length() {
    return len;
}
