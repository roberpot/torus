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

#include <network/packets/packetlist.h>
#include <network/socket.h>
#include <debug_support/info.h>
#include <core/config.h>
#include <shell.h>

#include <inaddr.h>


Packet_0xa8::Packet_0xa8()
{
    std::stringstream ss;
    uword_t serversCount = 1;
    uword_t serverIndex = 1;
    std::stringstream serverName;
    serverName << "TestServer"; //TODO: Send real Server's name
    t_ubyte serverPercentFull = 0;
    t_ubyte serverTimeZone = 0;


    set_packet_id(0xa8); //packet_id
    init_length();
    //write_word(0);    //Skip packet's size since it's being inserted at the end
    write_byte(0xFF);   // ?

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
    std::string ip = "127.0.0.1";
    in_addr addr;
    addr.S_un.S_addr = inet_addr(ip.c_str());

    bool reverse_ip = true;
    if (reverse_ip)
    {
        // Clients less than 4.0.0 require IP to be sent in reverse
        write_byte(addr.S_un.S_addr & 0xFF);
        write_byte((addr.S_un.S_addr >> 8) & 0xFF);
        write_byte((addr.S_un.S_addr >> 16) & 0xFF);
        write_byte((addr.S_un.S_addr >> 24) & 0xFF);       
    }
    else
    {
        // Clients since 4.0.0 require IP to be sent in order
        write_byte((addr.S_un.S_addr >> 24) & 0xFF);
        write_byte((addr.S_un.S_addr >> 16) & 0xFF);
        write_byte((addr.S_un.S_addr >> 8) & 0xFF);
        write_byte(addr.S_un.S_addr & 0xFF);
    }
    //Clients older than 4.0.0 must receive IP in reversed order.
    
    /*for (udword_t o = 0; o < buffer.size(); ++o)
    {
        TORUSSHELLECHO("byte[" << o << "] = " << (int)buffer[o]);
    }*/
    //write_word((t_ubyte)buffer.size() +2, 2);
    write_length();
}

Packet_0xa8::~Packet_0xa8() {
    ADDTOCALLSTACK();
}