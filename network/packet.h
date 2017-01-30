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
    const t_udword length();
    const t_byte * dumps();
    virtual void loads(const t_byte *) = 0;
    virtual void loads(Socket * s) = 0;
    virtual ~Packet();
protected:
    t_byte * buffer;
    t_udword len;
};


Packet * packet_factory(Socket & s);

Packet * packet_factory(t_byte t);

Packet * packet_factory(const t_byte * buffer, t_udword len);

#endif //__TORUS_PACKET_H
