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

#ifndef __TORUS_GAME_MAP_TERRAIN_H
#define __TORUS_GAME_MAP_TERRAIN_H

#include "../../library/types.h"


// Enable byte packing.
#if defined(_WIN32) && defined(_MSC_VER)
    #pragma pack(1) // MSVC dependant pragma
    #define PACK_STRUCT
#else
    #define PACK_STRUCT __attribute__ ((packed)) // GCC/MinGW definition
#endif

#define TILEDATA_BLOCK_SIZE 32  ///< Size of each block in tiledata.mul file.

/**
 * @brief Map Terrain struct for old tiledata.mul
 * size = 26 bytes
*/
struct UOMapTerrain {
    t_udword flags = 0;
    t_uword id = 0;
    t_byte name[20];
} PACK_STRUCT;

/**
 * @brief Map Terrain struct for new tiledata.mul (HS+)
 *
 * size = 30 bytes
*/
struct UOMapTerrainHS {
    t_udword flags = 0;
    t_udword unk;
    t_uword id = 0;
    t_byte name[20];
} PACK_STRUCT;

/**
 * @brief Item's struct on tiledata.mul -> art.mul
*/
struct UOItem {
    t_dword flags = 0;
    t_byte weight = 0;
    t_byte quality = 0; // Layer for equipable items, Light ID for light sources
    t_uword unk = 0;
    t_byte unk1 = 0;
    t_byte quantity = 0;
    t_uword animid = 0; // animation related to this item, add 50000 / 60000 to get the gumps for the equipable item.
    t_byte unk2 = 0;
    t_byte hue = 0;
    t_uword unk3 = 0;
    t_byte height = 0;  // for containers this means their max storage.
    t_byte name[20];
} PACK_STRUCT;

// Disable packing.
#if defined(_WIN32) && defined(_MSC_VER)
    #pragma pack()
    #undef PACK_STRUCT
#else
    #undef PACK_STRUCT
#endif
#endif // __TORUS_GAME_MAP_TERRAIN_H
