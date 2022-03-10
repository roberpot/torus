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

#ifndef __TORUS_PACKET_H
#define __TORUS_PACKET_H

#include <cstring>
#include <string>
#include <library/types.h>
#ifdef _MSC_VER
#pragma warning(disable:4127)
#endif

#include <vector>

#define CHARACTERS_STRING_LENGTH 30

class PacketIn;

/**
 *  @brief: Generic interface for packets.
 */
class Packet {
protected:
    uint8_t* _buffer;                ///< Buffer of the packet.
    uword_t _current_buffer_length;///< Length of the buffer.
    uword_t _current_pos;          ///< Current position to read/write on the buffer.


    /**
     * @Brief   Creates a new buffer with the additional size specified.
     *
     * @param   len The increased size in bytes.
     */
    void _increase_buffer(uword_t len);

public:
    /** @brief   Default constructor. */
    Packet();
    virtual ~Packet();
    /**
     * @brief   Gets the length.
     *
     * @return  The lenght.
     */
    virtual const uword_t length() = 0;
    /**
     * @brief Gets the data in the buffer.
     * 
     * @return nullptr if the buffer is empty, the data otherwise.
     */
    const uint8_t*data();

    /**
    * @brief    Get this packet's ID.
    * 
    * @return The ID, if the buffer has it's ID already assigned.
    */
    const t_ubyte packet_id();

    std::vector<uint8_t> buffer_to_vector(const uint8_t* buffer, udword_t len);
};

/**
 * @brief   Packets generator.
 *
 * @param s    The Socket with the data to process.
 *
 * @return  Null if it fails, else a pointer to a Packet.
 */
PacketIn* packet_factory(udword_t id);

#ifdef _MSC_VER
#pragma warning(default:4127)
#endif
#endif //__TORUS_PACKET_H
