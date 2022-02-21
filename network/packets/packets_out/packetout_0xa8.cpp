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

#ifdef _WIN32
    #include <inaddr.h>
#endif // _WIN32
#ifdef __linux__
    #include <netinet/in.h>
#endif //_WIN32


void PacketOut_0xa8::set_data(Socket* s)
{
    std::stringstream ss;
    uword_t serversCount = 1;
    uword_t serverIndex = 1;
    std::stringstream serverName;
    serverName << "TestServer"; //TODO: Send real Server's name
    t_ubyte serverPercentFull = 0;
    t_ubyte serverTimeZone = 0;


    //write_word(0);    //Skip packet's size since it's being inserted at the end
    write_ubyte(0xFF);   // ?

    write_uword(serversCount);   // TODO: Write later, after filling all the servers

    //TODO: Send all servers in a loop
    write_uword(serverIndex);

    int i = (int)serverName.str().size();

    for (; i < 32; ++i)
    {
        serverName << '\0';
    }

    write_string(serverName.str());

    write_ubyte(serverPercentFull);
    write_ubyte(serverTimeZone);

    //t_byte *ip = toruscfg.net_addr;
    udword_t ip = s->get_ip();

    bool reverse_ip = true;
    if (reverse_ip)
    {
        // Clients less than 4.0.0 require IP to be sent in reverse
        write_ubyte(ip & 0xFF);
        write_ubyte((ip >> 8) & 0xFF);
        write_ubyte((ip >> 16) & 0xFF);
        write_ubyte((ip >> 24) & 0xFF);
    }
    else
    {
        // Clients since 4.0.0 require IP to be sent in order
        write_ubyte((ip >> 24) & 0xFF);
        write_ubyte((ip >> 16) & 0xFF);
        write_ubyte((ip >> 8) & 0xFF);
        write_ubyte(ip & 0xFF);
    }
    //Clients older than 4.0.0 must receive IP in reversed order.

}

PacketOut_0xa8::PacketOut_0xa8() : PacketOut(0xa8, true)
{
    
}

PacketOut_0xa8::~PacketOut_0xa8() {
    ADDTOCALLSTACK();
}