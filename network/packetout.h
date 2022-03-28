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

#ifndef __TORUS_PACKETOUT_H
#define __TORUS_PACKETOUT_H

#include <library/types.h>

class Socket;
namespace Packets
{

class PacketOut : public Packet
{
    bool _has_dynamic_length;
public:
    PacketOut(udword_t id, bool has_dynamic_length = false);
    virtual ~PacketOut();

    const uword_t length() override;

    /**
    * @brief    Writes the packet id at the position 0.
    * 
    * @param id The packet id.
    */
    void set_packet_id(udword_t id);

    void send(Socket* s);

    void write_byte(t_byte byte);
    void write_ubyte(t_ubyte ubyte);
    void write_wchar(wchar_t wchar);
    void write_word(word_t word);
    void write_uword(uword_t uword);
    void write_dword(dword_t dword);
    void write_udword(udword_t udword);
    void write_qword(qword_t qword);
    void write_uqword(uqword_t uqword);

    void write_string(const std::string& string, uword_t len = 0);
    void write_wstring(const std::wstring& wstring, uword_t len = 0);


private:
    /**
     * @brief Write the length with a size of 0, usefull for dynamic sized packets.
     */
    void _init_length();
    /**
     * @brief Writes into the second byte the current length of the packet.
     */
    void _write_length();
};

}

#endif //__TORUS_PACKETOUT_H
