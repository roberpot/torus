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


#ifndef __TORUS_GAME_ENUMS_H
#define __TORUS_GAME_ENUMS_H

enum class TalkMode    // Modes we can talk/bark in.
{
    SAY = 0,        // 0 = A character speaking.
    SYSTEM = 1,     // 1 = Display as system prompt
    EMOTE = 2,      // 2 = *smiles* at object (client shortcut: :+space)
    ITEM = 6,       // 6 = text labeling an item. Preceeded by "You see"
    NOSCROLL = 7,   // 7 = As a status msg. Does not scroll (as reported by the packet guides)
    WHISPER = 8,    // 8 = Only those close can here. (client shortcut: ;+space)
    YELL = 9,       // 9 = Can be heard 2 screens away. (client shortcut: !+space)
    SPELL = 10,     // 10 = Used by spells
    GUILD = 0xD,    // 13 = Used by guild chat (client shortcut: \)
    ALLIANCE = 0xE, // 14 = Used by alliance chat (client shortcut: shift+\)
    COMMAND = 0xF,  // 15 = GM command prompt
    ENCODED = 0xC0, // 192 = Encoded speech

    // Special talkmodes, used internally by Torus
    SOUND = 0xFE, // Used to check if a char can hear a sound.
    BROADCAST = 0xFF  // It will be converted to something else.
};

enum class Font
{
    BOLD,       // 0 - Bold Text = Large plain filled block letters.
    SHAD,       // 1 - Text with shadow = small gray
    BOLD_SHAD,  // 2 - Bold+Shadow = Large Gray block letters.
    NORMAL,     // 3 - Normal (default) = Filled block letters.
    GOTH,       // 4 - Gothic = Very large blue letters.
    ITAL,       // 5 - Italic Script
    SM_DARK,    // 6 - Small Dark Letters = small Blue
    COLOR,      // 7 - Colorful Font (Buggy?) = small Gray (hazy)
    RUNE,       // 8 - Rune font (Only use capital letters with this!)
    SM_LITE,    // 9 - Small Light Letters = small roman gray font.
    QTY
};

enum class BodyType
{
    BODY_HUMAN_MALE = 0x190,
    BODY_HUMAN_FEMALE = 0x191
};

#endif // __TORUS_GAME_ENUMS_H
