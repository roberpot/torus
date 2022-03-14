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

namespace Packets
{

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

uword_t PacketIn::receive(const uint8_t* data, const uword_t len)
{
    udword_t old_length = _current_buffer_length;


    uword_t expected_length = 0;
    uword_t readed_bytes = 0;
    //t_byte expected_length = 0;
    if (_has_dynamic_length)
    {
        if (_current_buffer_length == 0)
        {
            _increase_buffer(3);  //Ensure the buffer can store the data.
            auto raw_buffer = buffer_to_vector(data, len);
            memcpy(&_buffer[old_length], data, 3);
            auto copy_buffer = buffer_to_vector(_buffer, 3);
            // If a packet has dynamic length, we must trust the size received in the buffer.
            uword_t old_pos = _current_pos; //Store current pos, should be 0 ... but store anyway.
            _current_pos = 1;               // Move the cursor to position 1, so the next 2 bytes read are the length.
            expected_length = read_uword();     // Read the length.
            _current_pos = old_pos;         // Restore the cursor.

            // Delete the buffer to do a normal read without using weird codes to adjust the lengths (readed, expected, etc).
            delete[] _buffer;
            _buffer = nullptr;
        }
        else
        {
            expected_length = _current_buffer_length;
        }
    }
    else
    {
        expected_length = length();
    }

    uword_t new_len = expected_length;
    if (new_len > len && len > 0) // Don't allow this packet to read data out of buffer size.
    {
        new_len = len;
    }
    else if (len > new_len) // also, if the buffer has more data than the required for this packet, cut it up too.
    {
        new_len = len;
    }
    _increase_buffer(new_len);  //Ensure the buffer can store the data.
    memcpy(_buffer + old_length, data, new_len);
    readed_bytes += new_len;    //
    if (_current_buffer_length >= expected_length)
    {
        _is_complete = true;
        _current_pos = 1; // Move the cursor, so the id is not read when processing it's data in the packets' specific code.
    }
    TORUSSHELLECHO("Packet " << this << " receive data (0x" << hex(_buffer[0]) << ")[" << std::dec << _current_buffer_length << "] = " << std::endl << hex_dump_buffer(_buffer, _current_buffer_length));
    return readed_bytes;
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

t_byte PacketIn::read_byte()
{
    return t_byte(_buffer[_current_pos++]);
}

t_ubyte PacketIn::read_ubyte()
{
    return t_ubyte(_buffer[_current_pos++]);
}

word_t PacketIn::read_word()
{
    return word_t((_buffer[_current_pos++] << 8) |
                   _buffer[_current_pos++]);
}

uword_t PacketIn::read_uword()
{
    return uword_t((_buffer[_current_pos++] << 8) |
                    _buffer[_current_pos++]);
}

dword_t PacketIn::read_dword()
{
    return dword_t((_buffer[_current_pos++] << 24) |
                   (_buffer[_current_pos++] << 16) |
                   (_buffer[_current_pos++] << 8)  |
                   _buffer[_current_pos++]);
}

udword_t PacketIn::read_udword()
{
    return udword_t((_buffer[_current_pos++] << 24) |
                    (_buffer[_current_pos++] << 16) |
                    (_buffer[_current_pos++] << 8) |
                    _buffer[_current_pos++]);
}

void PacketIn::skip(const uword_t& bytes)
{
    _current_pos += bytes;
    if (_current_pos > _current_buffer_length)
    {
        _current_pos = _current_buffer_length;
    }
}

void PacketIn::rewind(const uword_t& bytes)
{
    if (_current_pos - bytes < 0)
    {
        _current_pos = 0;
    }
    else
    {
        _current_pos -= bytes;
    }
}

}