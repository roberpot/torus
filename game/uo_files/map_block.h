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

#ifndef _TORUS_GAME_MAP_BLOCK_H
#define _TORUS_GAME_MAP_BLOCK_H

#include "../../library/types.h"
#include "map_point.h"

#define UO_MAP_BLOCK_SIZE 8
#define UO_MAP_BLOCK_CELLS 64

/**
* @brief Struct found on map* files containing a set of 8x8 map tiles.
*
* Struct's size = 196 bytes per block = 8x8 tiles * UOMapPoint(3 bytes)
*/
struct UOMapBlock {
    t_uword header_lo = 0;  ///< unused
    t_uword header_hi = 0;  ///< unused
    UOMapPoint points[UO_MAP_BLOCK_CELLS];   ///< array of tiles
};
#endif //_TORUS_GAME_MAP_BLOCK_H