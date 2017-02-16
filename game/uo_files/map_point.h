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

#ifndef _TORUS_GAME_MAP_POINT_H
#define _TORUS_GAME_MAP_POINT_H

#include "../../core/types.h"

#if defined(_WIN32) && defined(_MSC_VER)
    #pragma pack(1) // MSVC dependant pragma
    #define PACK_STRUCT
#else
    #define PACK_STRUCT __attribute__ ((packed)) // GCC/MinGW definition
#endif

/*
* @brief struct used in map* files containing the info of a x,y map tile
* 
* Struct's size: 3 bytes (Must be packed by compiler).
*/
struct UOMapPoint {
    t_word terrain = 0;
    t_byte z = 0;
} PACK_STRUCT;

// Turn off structure packing.
#if defined(_WIN32) && defined(_MSC_VER)
    #pragma pack()
    #undef PACK_STRUCT
#else
    #undef PACK_STRUCT
#endif
#endif //_TORUS_GAME_MAP_POINT_H