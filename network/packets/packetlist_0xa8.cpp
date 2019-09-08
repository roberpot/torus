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

Packet_0xa8::Packet_0xa8()
{
    std::stringstream ss;
    t_uword serversCount = 1;
    t_uword serverIndex = 1;
    std::stringstream serverName;
    serverName << "TestServer"; //TODO: Send real Server's name
    t_ubyte serverPercentFull = 0;
    t_ubyte serverTimeZone = 0;
    set_packet_id(0xa8); //packet_id
    //write_word(46);
    write_byte(0x0);
    //write_word(0);    //Skip packet's size since it's being inserted at the end
    write_word(serversCount);   // TODO: Write later, after filling all the servers

    //TODO: Send all servers in a loop
    write_word(serverIndex);

    int i = (int)serverName.str().size();

    for (int out = 0; out < i; ++out)
    {
        t_ubyte chr = serverName.str().at(out);
        write_byte(chr);
    }
    for (; i < 32; ++i)
    {
        write_byte('\0');
    }
    write_byte(serverPercentFull);
    write_byte(serverTimeZone);

    //t_byte *ip = toruscfg.net_addr;
    t_udword ip = 16777343;  //127.0.0.1    //TODO: send real IP
    write_byte((ip >> 24) & 0xFF);
    write_byte((ip >> 16) & 0xFF);
    write_byte((ip >> 8) & 0xFF);
    write_byte(ip & 0xFF);
    //Clients older than 4.0.0 must receive IP in reversed order.
    
    for (int o = 0; o < buffer.size(); ++o)
    {
        TORUSSHELLECHO("byte[" << o << "] = " << (int)buffer[o]);
    }
    write_word((t_ubyte)buffer.size() +2, 2);
}

Packet_0xa8::~Packet_0xa8() {
    ADDTOCALLSTACK();
}