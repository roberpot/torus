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


namespace Packets
{
namespace In
{

using namespace ::Out;

const uword_t Packet_0xa0::length() {
    ADDTOCALLSTACK();
    return 3;
}

void Packet_0xa0::process(Socket* s) {
    ADDTOCALLSTACK();
    ++_current_pos;
    t_byte server_index = read_byte();
    if (server_index >= TBYTE_MAX) {
        server_index -= TBYTE_MAX;
    }

    // >= 1.26.00 clients list Gives us a 1 based index for some reason.
    if (server_index > 0)
        server_index--;

    //TODO: Server selection here, read IP and PORTS from configuration for the given server index.


    s->set_connection_state(ConnectionState::CONNECTIONSTATE_CHARLIST);
    ServerAccept*packet_server_accept = new ServerAccept();
    packet_server_accept->set_data(s, server_index);
    packet_server_accept->send(s);
}

}
}