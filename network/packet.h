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

#ifndef __TORUS_PACKET_H
#define __TORUS_PACKET_H

#include "../core/types.h"

class Socket;

class Packet {
public:
    const unsigned int length();
    const char * dumps();
    virtual void loads(const char *) = 0;
    virtual void loads(Socket * s) = 0;
    virtual ~Packet() = 0;
protected:
    char * buffer;
    unsigned int len;
};

Packet * packet_factory(BYTE t);

Packet * packet_factory(const char * buffer, unsigned int len);

#endif //__TORUS_PACKET_H
