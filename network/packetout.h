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

class PacketOut : public Packet
{
    bool _has_dynamic_length;
public:
    PacketOut(t_byte id, bool has_dynamic_length = false);
    virtual ~PacketOut();

    const udword_t length() override;

    /**
    * @brief    Writes the packet id at the position 0.
    * 
    * @param id The packet id.
    */
    void set_packet_id(t_byte id);

    /**
     * @brief   Bitwise left shift operator, used to export the buffered data into useable variables.
     *
     * @param   p   The Packet to write on.
     * @param   d   The data to write.
     *
     * @return  The shifted result.
     */
    template<typename T>
    friend PacketOut& operator<<(PacketOut& p, T& d);
    template<typename T>
    friend PacketOut& operator<<(PacketOut& p, T&& d);

    /**
     * @brief   Bitwise left shift operator, used to export the buffered data into an useable std::string.
     *
     * @param   p   The Packet to write on.
     * @param   s   The string to write.
     *
     * @return  The shifted result.
     */
    friend PacketOut& operator<<(PacketOut& p, std::string& s);
    friend PacketOut& operator<<(PacketOut& p, std::string&& s);

    void send(Socket* s);

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


template<typename T>
PacketOut& operator<<(PacketOut& p, T& d)
{
    p._increase_buffer(sizeof(T));
    if (p._current_pos + sizeof(T) > p.length())
    {
        //THROW_ERROR(NetworkError, "Trying to read " << sizeof(T) << " bytes to from " << hex(p._buffer[0]) << ", being currently in the position " << p._current_pos << " and with a total length of " << p.length() " bytes.");
        return p;
    }
    memcpy(&(p._buffer[p._current_pos]), &d, sizeof(T));
    p._current_pos += sizeof(T);
    return p;
}

template<typename T>
PacketOut& operator<<(PacketOut& p, T&& d)
{
    p << d;
    return p;
}

template<typename T>
PacketOut& operator<<(PacketOut& p, std::string& s)
{
    p._increase_buffer(udword_t(s.size()));
    if (p._current_pos + sizeof(T) > s.size())
    {
        //THROW_ERROR(NetworkError, "Trying to read " << sizeof(T) << " bytes to from " << hex(p._buffer[0]) << ", being currently in the position " << p._current_pos << " into a string with a total size of " << s.size() " bytes.");
        return p;
    }
    memcpy(&(p._buffer[p._current_pos]), &(s.data()), s.size());
    p._current_pos += s.size();
    return p;
}

template<typename T>
PacketOut& operator<<(PacketOut& p, std::string&& s)
{
    p << s;
    return p;
}

#endif //__TORUS_PACKETOUT_H
