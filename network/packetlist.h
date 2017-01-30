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


#ifndef __TORUS_PACKETLIST_H
#define __TORUS_PACKETLIST_H

#include "packet.h"

#define PACKET_CREATE_CHARACTER Packet_0x00

class Packet_0x00 : public Packet {
public:
    const t_udword length();
    const t_byte * dumps();
    void loads(const t_byte *);
    void loads(Socket * s);
    ~Packet();
private:
};

#endif //__TORUS_PACKETLIST_H
