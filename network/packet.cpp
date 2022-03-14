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
#include <network/packets/packetlist.h>

#include <library/string.h>
#include <core/errors.h>
#include <network/socket.h>
#include <shell.h>
#include <initializer_list>


namespace Packets
{
    using namespace In;
PacketIn* packet_factory(udword_t id ) {
    ADDTOCALLSTACK();
    PacketIn * p = nullptr;
    switch (t_ubyte(id)) {
        case 0x00:
            p = new CreateCharacter();
            break;
        case 0x02:
            p = new MovementRequest();
            break;
        case 0x06:
            p = new UseRequest();
            break;
        case 0x34:
            p = new QueryCharacter();
            break;
        case 0x5d:
            p = new PlayCharacter();
            break;
        case 0x73:
            p = new Ping();
            break;
        case 0x80:
            p = new LoginConnect();
            break;
        case 0x91:
            p = new ServerConnect();
            break;
        case 0xa0:
            p = new ServerSelect();
            break;
        case 0xbd:
            p = new ReportCliver();
            break;
        case 0xbf:
            p = new ExtendedCmd();    //TODO: Inner switch for each 0xBF subpacket.
            break;
        case 0xef:
            p = new ReportCliverNew();
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
        //TORUSSHELLECHO("Deleting packet 0x" << hex(_buffer[0]) << "(" << std::dec << _current_buffer_length << ") " << std::endl << hex_dump_buffer(data(), _current_buffer_length));
        delete[] _buffer;
        _buffer = nullptr;
    }
}

const uint8_t* Packet::data()
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

std::vector<uint8_t> Packet::buffer_to_vector(const uint8_t* buffer, udword_t len)
{
    std::vector<uint8_t> vec;
    for (udword_t i = 0; i < len; ++i)
    {
        vec.push_back(buffer[i]);
    }
    return vec;
}

void Packet::_increase_buffer(uword_t len)
{
    //TORUSSHELLECHO("Increasing buffer by " << len << " bytes, current size = " << _current_buffer_length)
    uword_t new_size = _current_buffer_length + len;
    uint8_t* tmp_buffer = new uint8_t[new_size];
    memset(tmp_buffer, 0, new_size);
    if (_buffer != nullptr)
    {
        memcpy(tmp_buffer, _buffer, new_size);
        delete[] _buffer;
    }
    _buffer = tmp_buffer;
    _current_buffer_length = new_size;
}

}