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


#include <network/packetin.h>


PacketIn::PacketIn() :
    _is_complete(false)
{
}

PacketIn::~PacketIn()
{
}

const udword_t PacketIn::length()
{
    return _current_buffer_length;
}

const udword_t PacketIn::current_length()
{
    return _current_buffer_length;
}

void PacketIn::receive(const t_byte* data, const udword_t len)
{
    _increase_buffer(len);  //Ensure the buffer can store the data.
    memcpy(_buffer + _current_buffer_length, data, len);
    _current_buffer_length += len;
}

bool PacketIn::is_complete()
{
    return _is_complete;
}

void PacketIn::_increase_buffer(const udword_t& len)
{
    udword_t new_size = _current_buffer_length + len;
    t_byte *tmp_buffer = new t_byte[new_size];
    if (_buffer != nullptr)
    {
        memcpy(tmp_buffer, _buffer, _current_buffer_length);
        delete _buffer;
    }
    _buffer = tmp_buffer;
    _current_buffer_length = new_size;
}
