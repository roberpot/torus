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

#include <network/packets/packetlist.h>
#include <network/socket.h>
#include <debug_support/info.h>
#include <core/torus.h>
#include <game/client.h>


namespace Packets
{
namespace In
{

const uword_t Packet_0xad::length() {
    return _current_buffer_length;
}

void Packet_0xad::process(Socket* s) {
    ADDTOCALLSTACK();
    TalkMode talk_mode = TalkMode(read_byte());
    uword_t color = read_uword();
    Font font = Font(read_word());
    std::string lang = read_string(4);
    if (int(talk_mode) & int(TalkMode::ENCODED)) {
        udword_t remaining = get_remaining_length();
        /*  from ruosi.org:
        If Mode = 0xC0 then there are keywords(from speech.mul) present.
        Keywords are using in UO since 2.0.7 client.
        Keywords:   The first 12 bits = the number of keywords present.
                    The keywords are included right after this, each one is 12 bits also.
                    The keywords are padded to the closest byte.
                        For example, if there are 2 keywords, it will take up 5 bytes. 12 bits for the number, and 12 bits for each keyword. 12 + 12 + 12 = 36. Which will be padded 4 bits to 40 bits or 5 bytes.
        */
        udword_t count = (read_uword() & 0xFFF0) >> 4;
        rewind(2);
        count = (count + 1) * 12;
        udword_t to_skip = count / 8;
        if (count % 8 > 0) {
            ++to_skip;
        }

        if (to_skip > remaining) {
            return;
        }

        skip(to_skip);
        std::string text = read_string(remaining - to_skip);
        s->get_client()->event_talk_ascii(talk_mode, color, font, text);
    }
    else {
        std::wstring text = read_wstring(get_remaining_length());
        s->get_client()->event_talk_unicode(talk_mode, color, font, text);
    }
}

}
}