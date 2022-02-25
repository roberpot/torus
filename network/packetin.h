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

#ifndef __TORUS_PACKETIN_H
#define __TORUS_PACKETIN_H

#include <core/errors.h>
#include <library/types.h>
#include <network/packet.h>
#include <network/socket.h>

#include <string>

class Socket;

/*
* @brief    Representation of a generic incoming packet.
*
*   Must be implemented by each incoming packet.
* 
*   With normal behaviour, implementations only need to override the virtual methods 
*       "void process(Socket *s)" and "const udword_t length() override",
        the rest of the functionality is done here.
* 
*   When a packet receives data, it's buffer gets automatically increased
*       (May be a new packet, or an existing one receiving more data)
* 
*   When the lenght of the receipt data reaches the size of "length()",
*       the packet is marked as complete and inserted into the incoming queue
*       from the related socket, prepared to call process ASAP.
*/
class PacketIn : public Packet
{
    bool _is_complete;            ///< The full data for this packet has been received.

// Child classes implementations:
public:
    /**
    * @brief    Expected length for the packet.
    * 
    * @return The length.
    */
    virtual const uword_t length() = 0;
    /**
     *  @brief The packet has been received, process it's data.
     *
     *  @param s The socket that received this packet.
     */
    virtual void process(Socket* s) = 0;

// Class related:
public:
    PacketIn();
    virtual ~PacketIn();

    /**
     * @brief Retrieves the length of the received data.
     *
     * @return The length. 
     */
    const udword_t current_length();

    /**
     * @brief    Load a chunk of bytes into the buffer.
     * 
     * @param    data    The chunk of bytes to insert into the packet.
     * @paran    len     The length of the received data.
     * 
     * The receipt data may not form the full packet, yet.
     */
    void receive(const t_byte *data, const uword_t len);
    
    /**
     * @brief Completion status of the packet.
     * 
     * @return True if the data has been completelly received.
     */
    bool is_complete();

    template<typename T>
    /**
     * @brief   Bitwise right shift operator, used to export the buffered data into useable variables.
     *
     * @param   p   The Packet to read from.
     * @param   d   The data into which to write.
     *
     * @return  The shifted result.
     */
    friend PacketIn& operator>>(PacketIn& p, T& d);

    /**
     * @brief   Bitwise right shift operator, used to export the buffered data into an useable std::string.
     *
     * @param   p   The Packet to read from.
     * @param   s   The string into which to write (must have it's length assigned).
     *
     * @return  The shifted result.
     */
    friend PacketIn& operator>>(PacketIn& p, std::string &s);
    friend PacketIn& operator>>(PacketIn& p, std::string &&s);

    private:
};

template<typename T>
PacketIn& operator>>(PacketIn& p, T& d)
{
    if (p._current_pos + sizeof(T) > p.length())
    {
        //THROW_ERROR(NetworkError, "Trying to read " << sizeof(T) << " bytes to from " << hex(p._buffer[0]) << ", being currently in the position " << p._current_pos << " and with a total length of " << p.length() " bytes.");
        return p;
    }
    memcpy(&d, &(p._buffer[p._current_pos]), sizeof(T));
    p._current_pos += sizeof(T);
    return p;
}

template<typename T>
PacketIn& operator>>(PacketIn& p, std::string &s)
{
    if (p._current_pos + sizeof(T) > s.size())
    {
        //THROW_ERROR(NetworkError, "Trying to read " << sizeof(T) << " bytes to from " << hex(p._buffer[0]) << ", being currently in the position " << p._current_pos << " into a string with a total size of " << s.size() " bytes.");
        return p;
    }
    memcpy(s.data(), &(p._buffer[p._current_pos]), s.size());
    p._current_pos += s.size();
    return p;
}

template<typename T>
PacketIn& operator>>(PacketIn& p, std::string &&s)
{
    if (p._current_pos + sizeof(T) > s.size())
    {
        //THROW_ERROR(NetworkError, "Trying to read " << sizeof(T) << " bytes to from " << hex(p._buffer[0]) << ", being currently in the position " << p._current_pos << " into a string with a total size of " << s.size() " bytes.");
        return p;
    }
    memcpy(s.data(), &(p._buffer[p._current_pos]), s.size());
    p._current_pos += s.size();
    return p;
}


#endif //__TORUS_PACKETIN_H
