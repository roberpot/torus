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


PacketIn::PacketIn(bool has_dynamic_length) :
    _is_complete(false),
    _has_dynamic_length(has_dynamic_length)
{
}

PacketIn::~PacketIn()
{
}

const udword_t PacketIn::current_length()
{
    return _current_buffer_length;
}

void PacketIn::receive(const t_byte* data, const uword_t len)
{
    udword_t old_length = _current_buffer_length;
    _increase_buffer(len);  //Ensure the buffer can store the data.
    memcpy(_buffer + old_length, data, len);

    uword_t expected_length = 0;
    if (_has_dynamic_length)
    {
        // If a packet has dynamic length, we must trust the size received in the buffer.
        uword_t old_pos = _current_pos; //Store current pos, should be 0 ... but store anyway.
        _current_pos = 1;               // Move the cursor to position 1, so the next 2 bytes read are the length.
        (*this) >> expected_length;     // Read the length.
        _current_pos = old_pos;         // Restore the cursor.
    }
    else
    {
        expected_length = length();
    }
    if (_current_buffer_length >= expected_length)
    {
        _is_complete = true;
        _current_pos = 1; // Move the cursor, so the id is not read when processing it's data in the packets' specific code.
    }
}

bool PacketIn::is_complete()
{
    return _is_complete;
}

void PacketIn::read_string(std::string& str, uword_t len)
{
    if (len == 0)
    {
        len = uword_t(str.size());
    }
    if (_current_pos + len > length())
    {
        //THROW_ERROR(NetworkError, "Trying to read " << sizeof(T) << " bytes to from " << hex(p._buffer[0]) << ", being currently in the position " << p._current_pos << " into a string with a total size of " << s.size() " bytes.");
        return;
    }
    str.clear();
    str.resize(len);
    memcpy(str.data(), &(_buffer[_current_pos]), len);
    _current_pos += len;
}
