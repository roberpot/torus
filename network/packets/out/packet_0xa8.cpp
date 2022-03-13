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
#include <network/networkmanager.h>
#include <debug_support/info.h>
#include <core/config.h>
#include <shell.h>

#ifdef _WIN32
    #include <inaddr.h>
#endif // _WIN32
#ifdef __linux__
    #include <netinet/in.h>
#endif //_WIN32


namespace Packets
{
namespace Out
{

void Packet_0xa8::set_data(Socket* s)
{
    UNREFERENCED_PARAMETER(s);
    std::stringstream ss;
    uword_t servers_count = uword_t(toruscfg._game_servers.size());
    t_ubyte serverPercentFull = 0;
    t_ubyte serverTimeZone = 0;


    //write_word(0);    //Skip packet's size since it's being inserted at the end
    write_ubyte(0xFF);   // ?

    write_uword(servers_count);   // TODO: Write later, after filling all the servers

    //TODO: Send all servers in a loop
    for (uword_t server_id = 0; server_id < servers_count; ++server_id)
    {

        write_uword(server_id + 1);

        ServerInfo server_info(toruscfg._game_servers[server_id]);

        std::stringstream serverName;
        serverName << server_info.name; //TODO: Send real Server's name
        int i = (int)serverName.str().size();

        for (; i < 32; ++i)
        {
            serverName << '\0';
        }

        write_string(serverName.str(), 32);

        serverPercentFull = 0;  //TODO
        serverTimeZone = 0;
        write_ubyte(serverPercentFull);
        write_ubyte(serverTimeZone);

        std::vector<std::string> ip = split(server_info.ip, '.');  //127.0.0.1    //TODO: send real IP


        /*sockaddr_in inadd;
        inadd.sin_family = AF_INET;
        inadd.sin_addr.s_addr = inet_addr("84.122.110.73");
        udword_t ip = inadd.sin_addr.s_addr;  //127.0.0.1    //TODO: send real IP*/
        //Clients older than 4.0.0 must receive IP in reversed order.
        bool reverse_ip = true;
        if (reverse_ip)
        {
            // Clients less than 4.0.0 require IP to be sent in reverse
            write_ubyte(atoi(ip[3].c_str()) & 0xFF);
            write_ubyte(atoi(ip[2].c_str()) & 0xFF);
            write_ubyte(atoi(ip[1].c_str()) & 0xFF);
            write_ubyte(atoi(ip[0].c_str()) & 0xFF);
        }
        else
        {
            // Clients since 4.0.0 require IP to be sent in order
            write_ubyte(atoi(ip[0].c_str()) & 0xFF);
            write_ubyte(atoi(ip[1].c_str()) & 0xFF);
            write_ubyte(atoi(ip[2].c_str()) & 0xFF);
            write_ubyte(atoi(ip[3].c_str()) & 0xFF);
        }
    }
}

}
}