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

#ifndef __TORUS_NPACKET_H
#define __TORUS_NPACKET_H

#include <cstring>
#include <string>
#include <library/types.h>
#ifdef _MSC_VER
#pragma warning(disable:4127)
#endif

class PacketIn;

/*
*  @brief: Generic interface for packets.
*/
class NPacket {
protected:
    t_byte *_buffer;                ///< Buffer of the packet.
    udword_t _current_buffer_length;///< Length of the buffer.
    udword_t _current_pos;          ///< Current position to read/write on the buffer.
public:
    /** @brief   Default constructor. */
    NPacket();
    virtual ~NPacket();
    /**
     * @brief   Gets the length.
     *
     * @return  The lenght.
     */
    virtual const udword_t length() = 0;
    const t_byte *data();

};

/**
 * @brief   Packets generator.
 *
 * @param s    The Socket with the data to process.
 *
 * @return  Null if it fails, else a pointer to a Packet.
 */
PacketIn* packet_factory(const t_byte &id);

#ifdef _MSC_VER
#pragma warning(default:4127)
#endif
#endif //__TORUS_NPACKET_H
