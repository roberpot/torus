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

#include <library/system_headers.h>
#include <core/errors.h>
#include <library/types.h>
#include <network/packet.h>

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
    const udword_t current_length() const;

    /**
     * @brief    Load a chunk of bytes into the buffer.
     * 
     * @param    data    The chunk of bytes to insert into the packet.
     * @paran    len     The length of the received data.
     * 
     * The receipt data may not form the full packet, yet.
     */
    const uword_t receive(const uint8_t*data, const uword_t len);
    
    /**
     * @brief Completion status of the packet.
     * 
     * @return True if the data has been completelly received.
     */
    const bool is_complete() const;

protected:

    /**
     * @brief Get the remaining length to read.
     * 
     * @return The unreaded length in bytes.
     */
    const uword_t get_remaining_length() const;

    /**
     * @brief Read a string from the buffer.
     * 
     * @param len The length in bytes to read from the buffer.
     * @return The newly generated string with the text.
     */

    const std::string read_string(uword_t len = 0);

    /**
     * @brief Read a wstring from the buffer.
     *
     * @param len The length in bytes to read from the buffer.
     * @return The newly generated wstring with the text.
     */
    const std::wstring read_wstring(uword_t len = 0);

    /**
     * @brief Reads a bool from the buffer.
     *
     * @return The bool.
     */
    bool read_bool();

    /**
     * @brief Reads a signed byte from the buffer.
     * 
     * @return The byte.
     */
    const t_byte read_byte();

    /**
     * @brief Reads an unsigned byte from the buffer.
     *
     * @return The byte.
     */
    const t_ubyte read_ubyte();

    /**
     * @brief Read a signed word_t (2 bytes) from the buffer.
     * 
     * @return The value.
     */
    const word_t read_word();

    /**
     * @brief Read an unsigned word_t (2 bytes) from the buffer.
     *
     * @return The value.
     */
    const uword_t read_uword();

    /**
     * @brief Read a wide character (2 bytes) from the buffer.
     *
     * @return The value.
     */
    const wchar_t read_wchar();

    /**
     * @brief Read a signed dword_t (4 bytes) from the buffer.
     *
     * @return The value.
     */
    const dword_t read_dword();

    /**
     * @brief Read an unsigned dword_t (4 bytes) from the buffer.
     *
     * @return The value.
     */
    const udword_t read_udword();

    /**
     * @brief Skip a given number of bytes of the buffer.
     * 
     * @param bytes The number of bytes to skip.
     */
    void skip(const uword_t& bytes);

    /**
     * @brief Moves the buffer's cursor backwards, allowing to read again at certain positions.
     * 
     * @param bytes The number of bytes to rewind.
     * 
     * 
     */
    void rewind(const uword_t& bytes);

};

}


#endif //__TORUS_PACKETIN_H
