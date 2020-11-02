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
#include <vector>
#include <library/types.h>
#ifdef _MSC_VER
#pragma warning(disable:4127)
#endif

class Socket;

class Packet {
protected:
    std::vector<t_byte> buffer;    ///< Buffer of this packet.
public:
    /** @brief   Default constructor. */
    Packet();
    /**
     * @brief   Gets the length.
     *
     * @return  The lenght.
     */
    const udword_t length();

    void init_length();
    void write_length();
    /**
     * @brief   Dumps the buffer.
     *
     * @return  A pointer to the buffer.
     */
    virtual const t_byte * dumps();
    /**
     * @brief   Loads the given parameter 1.
     *
     * @param parameter1    The first parameter.
     */
    virtual void loads(const t_byte *) = 0;
    /**
     * @brief   Loads the given Socket.
     *
     * @param  s    If non-null, the Socket to process.
     */
    virtual void loads(Socket * s) = 0;
    /**
     * @brief   Send this message.
     *
     * @param  s    The Socket to process.
     */
    void send(Socket * s);
    /** @brief   Destructor. */
    virtual ~Packet();
    /**
     * @brief   Sets packet identifier.
     *
     * @param id    The identifier.
     */
    void set_packet_id(t_ubyte id);

    void write_string(std::string &str, int len);
    void write_bool(bool val, int pos = -1);
    void write_byte(t_byte val, int pos = -1);
    void write_ubyte(t_ubyte val, int pos = -1);
    void write_word(word_t val, int pos = -1);
    void write_uword(uword_t val, int pos = -1);
    void write_dword(dword_t val, int pos = -1);
    void write_udword(udword_t val, int pos = -1);
    void write_qword(qword_t val, int pos = -1);
    void write_uqword(uqword_t val, int pos = -1);

    void print(std::string ioType);

private:
};
/**
 * @brief   Packets generator.
 *
 * @param s    The Socket with the data to process.
 *
 * @return  Null if it fails, else a pointer to a Packet.
 */
Packet * packet_factory(Socket & s);

//Packet * packet_factory(t_byte t);

//Packet * packet_factory(const t_byte * buffer, udword_t len);

#ifdef _MSC_VER
#pragma warning(default:4127)
#endif
#endif //__TORUS_PACKET_H
