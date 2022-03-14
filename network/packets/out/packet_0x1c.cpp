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
#include <debug_support/info.h>

#include <game/artifact.h>
#include <game/char.h>
#include <game/enums.h>

namespace Packets
{
namespace Out
{

void Packet_0x1c::set_data(const std::string& text, Artifact* target, const word_t& hue, const TalkMode& talk_mode, const Font &font)
{
    if (target)
    {
        write_udword(target->get_uid().get_uid());
    }
    else
    {
        write_udword(0xFFFFFFFF);
    }

    uword_t body = 0xFFFF;
    if (target->get_uid().is_char())
    {
        body = uword_t(static_cast<Char*>(target)->get_body());
    }
    write_uword(body);
    write_byte(uword_t(talk_mode));
    write_uword(hue);
    write_uword(uword_t(font));
    write_string(text, TEXT_LENGTH);
}

}
}