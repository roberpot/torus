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
    udword_t _current_pos;
    bool _full_received;           ///< The full data for this packet has been received.
public:
    /** @brief   Default constructor. */
    Packet();
    /**
     * @brief   Gets the length.
     *
     * @return  The lenght.
     */
    virtual const udword_t length();

    void init_length();
    void write_length();
    bool full_received();
    udword_t id();
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
    virtual void loads(Socket *s, const udword_t &len);
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

    virtual void receive(Socket *s);
    void read_string(std::string &target, udword_t len);

    void write_from_paste(std::string paste);

    template<typename T>
    /**
     * @brief   Bitwise right shift operator, used to export the data received to a Packet's buffer.
     *
     * @param   s   The Socket to process.
     * @param   d   The data to process.
     *
     * @return  The shifted result.
     */
    friend Packet& operator>>(Packet& s, T& d);
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


template<typename T>
Packet& operator>>(Packet& p, T& d) {
    //ADDTOCALLSTACK();
    udword_t len = sizeof(T);
    if (p._current_pos + len <= p.length())
    {
        std::vector<t_ubyte> tmp;
        tmp.resize(len);
        std::vector<t_byte>::iterator it_begin = p.buffer.begin() + p._current_pos;
        std::vector<t_byte>::iterator it_end = p.buffer.begin() + p._current_pos + len;
        std::copy(it_begin, it_end, tmp.data());
        //p.buffer.insert(tmp.end(), it_begin, it_end);
        //tmp.insert(p.buffer.begin() + p._current_pos, len);
        memcpy(&d, tmp.data(), len);
        p._current_pos += len;
    }
    return p;
}
#ifdef _MSC_VER
#pragma warning(default:4127)
#endif
#endif //__TORUS_PACKET_H
