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

#include <network/NPacket.h>
#include <network/packetin.h>
#include <network/Packets/Packetlist.h>

#include <library/string.h>
#include <core/errors.h>
#include <network/socket.h>
#include <shell.h>
#include <initializer_list>


PacketIn* packet_factory( const t_byte &id ) {
    ADDTOCALLSTACK();
    PacketIn * p = nullptr;
    switch(id) {
        /*case 0x00:
            p = new Packet_0x00(0x0);
            break;
        case 0xa0:
            p = new Packet_0xa0(0xa0);
            break;
        case 0xef: 
            p = new Packet_0xef(0xef);
            break;
        case 0x80:
            p = new Packet_0x80(0x80);
            break;
        case 0xa8:
            p = new Packet_0xa8(0xa8);
            break;
        case 0x91:
            p = new Packet_0x91(0x91);
            break;
        case 0x73:
            p = new Packet_0x73(0x73);
            break;*/
        default:
            TORUSSHELLECHO("Unknown packet code 0x" << std::hex << id << std::dec << ".");
            //THROW_ERROR(NetworkError, "Unknown packet code 0x" << std::hex << (udword_t)t << std::dec << ".");
    }
    return p;
}

NPacket::NPacket() :
    _buffer(nullptr),
    _current_buffer_length(0),
    _current_pos(0)
{
    ADDTOCALLSTACK();
}

NPacket::~NPacket()
{
    if (_buffer != nullptr)
    {
        delete _buffer;
    }
}

const t_byte* NPacket::data()
{
    return _buffer;
}
