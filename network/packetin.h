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

namespace Packets
{
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
    bool _has_dynamic_length;

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
    PacketIn(bool has_dynamic_length = false);
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
    uword_t receive(const uint8_t*data, const uword_t len);
    
    /**
     * @brief Completion status of the packet.
     * 
     * @return True if the data has been completelly received.
     */
    bool is_complete();


    /**
     * @brief Read a string from the buffer.
     * 
     * @param str The string in which to 
     * @param len The length in bytes of the buffer.
     */
    void read_string(std::string &str, uword_t len = 0);
    std::string read_string(uword_t len = 0);

    /**
     * @brief Read a wstring from the buffer.
     *
     * @param len The length in bytes of the buffer.
     * @return The wstring.
     */
    std::wstring read_wstring(uword_t len = 0);

    /**
     * @brief Reads a signed byte from the buffer.
     * 
     * @return The byte.
     */
    t_byte read_byte();

    /**
     * @brief Reads a signed byte from the buffer.
     *
     * @return The byte.
     */
    t_ubyte read_ubyte();
    word_t read_word();
    uword_t read_uword();
    dword_t read_dword();
    udword_t read_udword();

    void skip(const uword_t& bytes);
    void rewind(const uword_t& bytes);

    private:
};

}


#endif //__TORUS_PACKETIN_H
