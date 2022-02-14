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

#include <network/packet.h>
#include <network/packetin.h>
#include <network/Packets/Packetlist.h>

#include <library/string.h>
#include <core/errors.h>
#include <network/socket.h>
#include <shell.h>
#include <initializer_list>


PacketIn* packet_factory( t_ubyte id ) {
    ADDTOCALLSTACK();
    PacketIn * p = nullptr;
    switch(id) {
        case 0x00:
            p = new PacketIn_0x00();
            break;
        case 0xa0:
            p = new PacketIn_0xa0();
            break;
        case 0xef: 
            p = new PacketIn_0xef();
            break;
        case 0x80:
            p = new PacketIn_0x80();
            break;
        case 0x91:
            p = new PacketIn_0x91();
            break;
        case 0x73:
            p = new PacketIn_0x73();
            break;
        default:
            TORUSSHELLECHO("Unknown packet code 0x" << hex(id) << ".");
            //THROW_ERROR(NetworkError, "Unknown packet code 0x" << std::hex << (udword_t)t << std::dec << ".");
    }
    return p;
}

Packet::Packet() :
    _buffer(nullptr),
    _current_buffer_length(0),
    _current_pos(0)
{
    ADDTOCALLSTACK();
}

Packet::~Packet()
{
    if (_buffer != nullptr)
    {
        TORUSSHELLECHO("Deleting packet 0x" << hex(_buffer[0]) << " [" << _current_buffer_length << "] " << std::endl << hex_dump_buffer(data(), _current_buffer_length));
        delete[] _buffer;
        _buffer = nullptr;
    }
}

const t_byte* Packet::data()
{
    return _buffer;
}

const t_ubyte Packet::packet_id()
{
    t_ubyte id = TUBYTE_MAX;
    if (_buffer)
    {
        id = _buffer[0];
    }
    return id;
}

void Packet::_increase_buffer(udword_t len)
{
    udword_t new_size = _current_buffer_length + len;
    t_byte* tmp_buffer = new t_byte[new_size];
    memset(tmp_buffer, 0, new_size);
    if (_buffer != nullptr)
    {
        memcpy(tmp_buffer, _buffer, _current_buffer_length);
        delete[] _buffer;
    }
    _buffer = tmp_buffer;
    _current_buffer_length = new_size;
}