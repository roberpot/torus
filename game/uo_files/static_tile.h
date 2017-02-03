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

#ifndef _TORUS_GAME_STATIC_TILE_H
#define _TORUS_GAME_STATIC_TILE_H

#include "../../core/types.h"

struct UOStaticTile {
    t_word tileid = 0;
    t_byte x = 0;
    t_byte y = 0;
    t_byte z = 0;
    t_word flags = 0;
    // ?
    // LOBYTE(flags) = Blocking flags for this item. (CAN_I_BLOCK)
    // HIBYTE(flags) = Height of this object.
    // TODO Confirm these functions works as supossed.
    t_word get_flags() {
        return flags &~0xFF00;
    }
    t_word get_height() {
        return flags &~0x00FF;
    }
};

#endif //_TORUS_GAME_STATIC_TILE_H