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
#include <initializer_list>


Packet * packet_factory(Socket & s) {
    ADDTOCALLSTACK();
    Packet * p = nullptr;
    if (!s.get_client())
    {
        return p;
    }
    t_ubyte t = 0;
    s >> t;

    if (t == TUBYTE_MAX) // Invalid recv, 0 or -1, catch it here and close the socket
    {
        s.set_closing();
        return p;
    }

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
        case 0xa8:
            p = new Packet_0xa8();
            break;
        case 0x91:
            p = new Packet_0x91();
            break;
        case 0x73:
            p = new Packet_0x73();
            break;
        default:
            TORUSSHELLECHO("Unknown packet code 0x" << std::hex << (udword_t)t << std::dec << ".");
            //THROW_ERROR(NetworkError, "Unknown packet code 0x" << std::hex << (udword_t)t << std::dec << ".");
    }
    if (p)
    {
        p->set_packet_id(t);
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
    _current_pos = 0;
    _full_received = false;
}

const t_byte * Packet::dumps() {
    //return buffer;    
    return buffer.data();
}

void Packet::loads(Socket *s, const udword_t& len)
{
    ADDTOCALLSTACK();
    udword_t current_length = udword_t(buffer.size());
    if (current_length < length()) {
        buffer.resize(length());
    }

    if (current_length + len < length())
    {
        TORUSSHELLECHO("Received data not enough, " << current_length + len << " out of " << length() << " for packet 0x" << id());
    }
    else if (current_length + len == length())
    {
        TORUSSHELLECHO("Received all bytes (" << length() << ") for packet 0x" << id());
    }
    if (current_length + len <= length())
    {
        std::vector<t_byte> tmp;
        tmp.resize(len);
        memcpy(tmp.data(), s->data(), len);

        for (size_t i = 0; i < tmp.size(); ++i) //FIXME: Please! ugly code!
        {
            buffer[current_length + i] =  tmp[i];
        }
    }

    if (buffer.size() >= length())
    {
        _full_received = true;
        receive(s);
    }
}

const udword_t Packet::length() {
    return (udword_t)buffer.size();// len;
}

void Packet::init_length()
{
    write_word(0);
}

void Packet::write_length()
{
    buffer[2] = t_byte(buffer.size()); //TODO: Check if this works right
}

bool Packet::full_received()
{
    return _full_received;
}

udword_t Packet::id()
{
    udword_t id = TUDWORD_MAX;
    if (!buffer.empty())
    {
        id = buffer.at(0);
    }
    return id;
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
    std::string tmpStr(str);
    tmpStr.insert(0, ilen - str.size(), 0);
    tmpStr.insert(0, str.c_str());
    for (int i = 0; i < ilen; ++i) {
        buffer.emplace_back(tmpStr.at(i));
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

void Packet::print(std::string ioType)
{
    std::stringstream ss;
    if (buffer.empty())
    {
        return;
    }
    ss << ioType;
    ss << length();
    ss << " bytes in raw data for packet : 0x" << hex(buffer[0]) << "\n";
    for (udword_t i = 0; i < length(); ++i) {
        if (i % 16 == 0)
            ss << "\n";
        else
            ss << " | ";
        ss << hex(buffer[i]);
    }
    TORUSSHELLECHO(ss.str());
}

void Packet::read_string(std::string& target, udword_t len)
{
    if (_current_pos + len <= length())
    {
        target.resize(len);
        std::copy(buffer.begin() + _current_pos, buffer.begin() + _current_pos + len, target.data());
        _current_pos += len;
    }
}

void Packet::write_from_paste(std::string paste)
{
    buffer.clear();
    t_byte c[2];
    for (size_t i = 0; i < paste.size(); i += 2)
    {
        c[0] = paste[i];
        c[1] = paste[i + 1];
        buffer.push_back(*c);
    }
}

void Packet::receive(Socket *)
{
}
