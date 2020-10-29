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
#include <network/packets/packetlist.h>

#include <library/string.h>
#include <core/errors.h>
#include <network/socket.h>
#include <shell.h>



Packet * packet_factory(Socket & s) {
    ADDTOCALLSTACK();
    t_ubyte t = 0;
    s >> t;
    Packet * p = nullptr;
    switch(t) {
        case 0x00:
            p = new Packet_0x00();
            break;
        case 0xa0:
            p = new Packet_0xa0();
            break;
        case 0xef: 
            p = new Packet_0xef();
            break;
        case 0x80:
            p = new Packet_0x80();
            break;
        default:
            TORUSSHELLECHO("Unknown packet code 0x" << std::hex << (udword_t)t << std::dec << ".");
            //THROW_ERROR(NetworkError, "Unknown packet code 0x" << std::hex << (udword_t)t << std::dec << ".");
    }
    if (p != nullptr)
    {
        p->loads(&s);
    }
    return p;
}

/*Packet * packet_factory(t_byte t) {
    ADDTOCALLSTACK();
    switch(t) {
        default: THROW_ERROR(NetworkError, "Unknown packet code 0x" << std::hex << (udword_t)t << std::dec << ".");
    }
    //return 0;
}*/

/*Packet * packet_factory(const t_byte * buffer, udword_t len) {
    ADDTOCALLSTACK();
    switch(buffer[0]) {
        default: THROW_ERROR(NetworkError, "Unknown packet [" << len << "] " << std::hex << print_hex_buffer(buffer, len) << std::dec << ".");
    }
    //return 0;
}*/


Packet::Packet() {
    ADDTOCALLSTACK();
}

const t_byte * Packet::dumps() {
    //return buffer;    
    return buffer.data();
}

const udword_t Packet::length() {
    return (udword_t)buffer.size();// len;
}

void Packet::send(Socket * socket) {
    ADDTOCALLSTACK();
    //if (buffer) {
    if (buffer.size() > 0) {
        socket->write_packet(this);
    }
    else {
        THROW_ERROR(NetworkError, "Sending empty packet ");
    }
}

Packet::~Packet() {
    ADDTOCALLSTACK();
    //if (buffer) delete buffer;
    buffer.clear();
}

void Packet::set_packet_id(t_ubyte id) {
    ADDTOCALLSTACK();
    write_ubyte(id);
}

void Packet::write_string(std::string& str, int ilen)
{
    ADDTOCALLSTACK();
    buffer.reserve(buffer.size() + str.size());
    for (int i = 0; i < ilen; ++i) {
        buffer.emplace_back(str.at(i));
    }
    /*if (sizeof(T) == 4) {
        d = (((d & 0x000000ff) << 24) | ((d & 0x0000ff00) << 8) | ((d & 0x00ff0000) >> 8) | ((d & 0xff000000) >> 24));
    }*/
}
void Packet::write_bool(bool val, int pos)
{
    ADDTOCALLSTACK();
    buffer.reserve(buffer.size() + sizeof(val));
    if (pos >= 0)
    {
        buffer.insert(std::begin(buffer) + pos, (t_byte)val ? 1 : 0);
    }
    else
    {
        buffer.emplace_back((t_byte)val ? 1 : 0);
    }
}


void Packet::write_byte(t_byte val, int pos)
{
    ADDTOCALLSTACK();
    buffer.reserve(buffer.size() + sizeof(val));
    if (pos >= 0)
    {
        buffer.insert(std::begin(buffer) + pos, (t_byte)val);
    }
    else
    {
        buffer.emplace_back(val);
    }
}

void Packet::write_ubyte(t_ubyte val, int pos)
{
    ADDTOCALLSTACK();
    buffer.reserve(buffer.size() + sizeof(val));
    if (pos >= 0)
    {
        buffer.insert(std::begin(buffer) + pos, (t_byte)val);
    }
    else
    {
        buffer.emplace_back(val);
    }
}

void Packet::write_word(word_t val, int pos)
{
    ADDTOCALLSTACK();
    buffer.reserve(buffer.size() + sizeof(val));
    if (pos >= 0)
    {
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 8));
        buffer.insert(std::begin(buffer) + pos, (t_byte)val);
    }
    else
    {
        buffer.emplace_back((t_byte)(val >> 8));
        buffer.emplace_back((t_byte)val);
    }
}

void Packet::write_uword(uword_t val, int pos)
{
    ADDTOCALLSTACK();
    buffer.reserve(buffer.size() + sizeof(val));
    if (pos >= 0)
    {
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 8));
        buffer.insert(std::begin(buffer) + pos, (t_byte)val);
    }
    else
    {
        buffer.emplace_back((t_byte)(val >> 8));
        buffer.emplace_back((t_byte)val);
    }
}

void Packet::write_dword(dword_t val, int pos)
{
    ADDTOCALLSTACK();
    buffer.reserve(buffer.size() + sizeof(val));
    if (pos >= 0)
    {
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 24));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 16));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 8));
        buffer.insert(std::begin(buffer) + pos, (t_byte)val);
    }
    else
    {
        buffer.emplace_back((t_byte)(val >> 24));
        buffer.emplace_back((t_byte)(val >> 16));
        buffer.emplace_back((t_byte)(val >> 8));
        buffer.emplace_back((t_byte)val);
    }
}

void Packet::write_udword(udword_t val, int pos)
{
    ADDTOCALLSTACK();
    if (pos >= 0)
    {
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 24));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 16));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 8));
        buffer.insert(std::begin(buffer) + pos, (t_byte)val);
    }
    else
    {
        buffer.emplace_back((t_byte)(val >> 24));
        buffer.emplace_back((t_byte)(val >> 16));
        buffer.emplace_back((t_byte)(val >> 8));
        buffer.emplace_back((t_byte)val);
    }
}

void Packet::write_qword(qword_t val, int pos)
{
    ADDTOCALLSTACK();
    if (pos >= 0)
    {
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 56));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 48));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 40));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 32));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 24));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 16));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 8));
        buffer.insert(std::begin(buffer) + pos, (t_byte)val);
    }
    else
    {
        buffer.emplace_back((t_byte)(val >> 56));
        buffer.emplace_back((t_byte)(val >> 48));
        buffer.emplace_back((t_byte)(val >> 40));
        buffer.emplace_back((t_byte)(val >> 32));
        buffer.emplace_back((t_byte)(val >> 24));
        buffer.emplace_back((t_byte)(val >> 16));
        buffer.emplace_back((t_byte)(val >> 8));
        buffer.emplace_back((t_byte)val);
    }
}

void Packet::write_uqword(uqword_t val, int pos)
{
    ADDTOCALLSTACK();
    if (pos >= 0)
    {
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 56));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 48));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 40));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 32));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 24));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 16));
        buffer.insert(std::begin(buffer) + pos, (t_byte)(val >> 8));
        buffer.insert(std::begin(buffer) + pos, (t_byte)val);
    }
    else
    {
        buffer.emplace_back((t_byte)(val >> 56));
        buffer.emplace_back((t_byte)(val >> 48));
        buffer.emplace_back((t_byte)(val >> 40));
        buffer.emplace_back((t_byte)(val >> 32));
        buffer.emplace_back((t_byte)(val >> 24));
        buffer.emplace_back((t_byte)(val >> 16));
        buffer.emplace_back((t_byte)(val >> 8));
        buffer.emplace_back((t_byte)val);
    }
}

void Packet::print()
{
    std::stringstream ss;
    ss << "Sending ";
    ss << length();
    ss << " bytes in raw data:";
    for (udword_t i = 0; i < length(); ++i) {
        if (i % 10 == 0)
            ss << "\n";
        else
            ss << " | ";
        ss << hex(buffer[i]);
    }
    TORUSSHELLECHO(ss.str());
}
