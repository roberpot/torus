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
#include <network/packetout.h>
#include <network/socket.h>


namespace Packets
{

PacketOut::PacketOut(udword_t id, bool has_dynamic_length) :
    _has_dynamic_length(has_dynamic_length)
{
    ADDTOCALLSTACK();
    set_packet_id(id);
}

PacketOut::~PacketOut()
{
}

const uword_t PacketOut::length()
{
    return _current_buffer_length;
}

void PacketOut::set_packet_id(udword_t id)
{
    ADDTOCALLSTACK();
    if (id > TUBYTE_MAX)
    {
        THROW_ERROR(NetworkError, "Bad packet ID " << id);
    }
    write_byte(t_byte(id));
    if (_has_dynamic_length)
    {
        _init_length();
    }
}

void PacketOut::_init_length()
{
    ADDTOCALLSTACK();
    write_uword(uword_t(_current_buffer_length));
}

void PacketOut::_write_length()
{
    ADDTOCALLSTACK();
    _buffer[1] = t_byte(_current_buffer_length >> 8);
    _buffer[2] = t_byte(_current_buffer_length);
}

void PacketOut::send(Socket* s)
{
    ADDTOCALLSTACK();
    if (_has_dynamic_length)
    {
        _write_length();
    }
    s->write(this);
}

void PacketOut::write_byte(t_byte byte)
{
    ADDTOCALLSTACK();
    _increase_buffer(1);
    _buffer[_current_pos++] = byte;
}

void PacketOut::write_ubyte(t_ubyte ubyte)
{
    ADDTOCALLSTACK();
    _increase_buffer(1);
    _buffer[_current_pos++] = ubyte;
}

void PacketOut::write_word(word_t word)
{
    ADDTOCALLSTACK();
    _increase_buffer(2);
    _buffer[_current_pos++] = t_byte(word >> 8);
    _buffer[_current_pos++] = t_byte(word);
}

void PacketOut::write_uword(uword_t uword)
{
    ADDTOCALLSTACK();
    _increase_buffer(2);
    _buffer[_current_pos++] = t_byte(uword >> 8);
    _buffer[_current_pos++] = t_byte(uword);
}

void PacketOut::write_dword(dword_t dword)
{
    ADDTOCALLSTACK();
    _increase_buffer(4);
    _buffer[_current_pos++] = t_byte(dword >> 24);
    _buffer[_current_pos++] = t_byte(dword >> 16);
    _buffer[_current_pos++] = t_byte(dword >> 8);
    _buffer[_current_pos++] = t_byte(dword);
}

void PacketOut::write_udword(udword_t udword)
{
    ADDTOCALLSTACK();
    _increase_buffer(4);
    _buffer[_current_pos++] = t_byte(udword >> 24);
    _buffer[_current_pos++] = t_byte(udword >> 16);
    _buffer[_current_pos++] = t_byte(udword >> 8);
    _buffer[_current_pos++] = t_byte(udword);
}

void PacketOut::write_qword(qword_t qword)
{
    ADDTOCALLSTACK();
    _increase_buffer(8);
    _buffer[_current_pos++] = t_byte(qword >> 56);
    _buffer[_current_pos++] = t_byte(qword >> 48);
    _buffer[_current_pos++] = t_byte(qword >> 40);
    _buffer[_current_pos++] = t_byte(qword >> 32);
    _buffer[_current_pos++] = t_byte(qword >> 24);
    _buffer[_current_pos++] = t_byte(qword >> 16);
    _buffer[_current_pos++] = t_byte(qword >> 8);
    _buffer[_current_pos++] = t_byte(qword);
}

void PacketOut::write_uqword(uqword_t uqword)
{
    ADDTOCALLSTACK();
    _increase_buffer(8);
    _buffer[_current_pos++] = t_byte(uqword >> 56);
    _buffer[_current_pos++] = t_byte(uqword >> 48);
    _buffer[_current_pos++] = t_byte(uqword >> 40);
    _buffer[_current_pos++] = t_byte(uqword >> 32);
    _buffer[_current_pos++] = t_byte(uqword >> 24);
    _buffer[_current_pos++] = t_byte(uqword >> 16);
    _buffer[_current_pos++] = t_byte(uqword >> 8);
    _buffer[_current_pos++] = t_byte(uqword);
}

void PacketOut::write_string(const std::string& string, uword_t len)
{
    ADDTOCALLSTACK();
    _increase_buffer(len);
    size_t i = 0;
    for (; i < string.size(); ++i)
    {
        _buffer[_current_pos++] = string[i];
    }
    for (; i < size_t(len); ++i)
    {
        _buffer[_current_pos++] = '\0';
    }
}

}