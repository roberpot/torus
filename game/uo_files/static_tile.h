#ifndef _TORUS_GAME_STATIC_TILE_H
#define _TORUS_GAME_STATIC_TILE_H

#include "../../core/types.h"
#include "map.h"



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