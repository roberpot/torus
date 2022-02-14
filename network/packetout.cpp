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
#include <network/Packets/Packetlist.h>
#include <network/packetout.h>
#include <network/socket.h>

PacketOut::PacketOut(t_byte id, bool has_dynamic_length) :
    _has_dynamic_length(has_dynamic_length)
{
    set_packet_id(id);
}

PacketOut::~PacketOut()
{
}

const udword_t PacketOut::length()
{
    return _current_buffer_length;
}

void PacketOut::set_packet_id(t_byte id)
{
    _increase_buffer(1);
    _buffer[0] = id;
    if (_has_dynamic_length)
    {
        _init_length();
    }
}

void PacketOut::_init_length()
{
    *this << word_t(_current_buffer_length);
}

void PacketOut::_write_length()
{
    memcpy(&_buffer[1], &_current_buffer_length, sizeof(word_t));
}

void PacketOut::send(Socket* s)
{
    if (_has_dynamic_length)
    {
        _write_length();
    }
    s->write(this);
}
