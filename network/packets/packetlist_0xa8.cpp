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
#include "../../core/config.h"


const t_udword Packet_0xa8::length() {
    ADDTOCALLSTACK();
    return 0;
}

const t_byte * Packet_0xa8::dumps() {
    ADDTOCALLSTACK();
    return 0;
}

Packet_0xa8::Packet_0xa8()
{
    std::stringstream ss;
    buffer = new t_byte[255];
    memset((void*)buffer, 0, 255);
    int iLen;
    t_uword serversCount = 1;
    t_uword serverIndex = 1;
    std::stringstream serverName;
    serverName << "TestServer";
    t_ubyte serverPercentFull = 0;
    t_ubyte serverTimeZone = 0;
    (*this) << 0xa8; //packet_id
    (*this) << 0x00;
    (*this) << 45;
    (*this) << 0xFF;
    (*this) << 0x00;
    (*this) << serversCount;

    (*this) << 0x00;

    (*this) << serverIndex;
    //write_string(*serverName, 32);

    int i = serverName.str().size();

    for (int out = 0; out < i; ++out)
    {
        t_ubyte chr = serverName.str().at(out);
        (*this) << chr;
    }
    for (; i < 32; ++i)
    {
        (*this) << '\0';
    }
    (*this) << serverPercentFull;
    (*this) << serverTimeZone;

    //t_byte *ip = toruscfg.net_addr;
    t_udword ip = 16777343;  //127.0.0.1
    t_ubyte ip_1 = ((ip >> 24) & 0xFF);
    t_ubyte ip_2 = ((ip >> 16) & 0xFF);
    t_ubyte ip_3 = ((ip >> 8) & 0xFF);
    t_ubyte ip_4 = (ip & 0xFF);
    (*this) << ip_1;
    (*this) << ip_2;
    (*this) << ip_3;
    (*this) << ip_4;
    
    for (int i = 0; i < count; ++i)
    {
        TORUSSHELLECHO("byte[" << i << "] = " << (int)buffer[i]);
    }
    set_pos(2);
    (*this) << (t_word)count -1;
    //tmp_buffer << ss.str().size();
}

Packet_0xa8::~Packet_0xa8() {
    ADDTOCALLSTACK();
}