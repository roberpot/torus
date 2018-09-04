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

#ifndef __TORUS_GAME_MAP_H
#define __TORUS_GAME_MAP_H

#include "../../library/types.h"
#include "map_block.h"
#include <string>

enum MAPID {
    MAP_FELUCCA,
    MAP_TRAMMEL,
    MAP_ILSHENAR,
    MAP_MALAS,
    MAP_TOKUNO,
    MAP_TER_MUR,
    MAP_QTY
};

class Map {
private:
    t_uword _index; // Ingame Virtual ID
    t_uword _x;     // max X
    t_uword _y;     // max Y
    t_ubyte _fileid;    // map file in which this map is based on (255 = bad id).
    t_ubyte _sector_size;
    UOMapBlock *_blocks;
    std::string _filename;
    bool _is_valid;
public:
    Map();
    ~Map();
    /**
    * @brief Create the structure of this map.
    *
    * @param x Higher X tile for this map.
    * @param y Higher Y tile for this map.
    * @param file_id Map file used for this map.
    * @return false if the given file doesn't exist or can't be opened.
    */
    bool create(t_uword index, t_uword x, t_uword y, t_ubyte ss, t_ubyte file_id, std::string filename);
    /**
    * @brief Fills the Map with it's map file's data.
    *
    * @return success or fail;
    */
    bool init();
    /**
    * @brief Checks if this a valid map.
    *
    * @return false if not.
    */
    bool is_valid();
    /**
    * @brief Get the max X tile of this map.
    *
    * @return max X.
    */
    t_word get_max_x();
    /**
    * @brief Get the max Y tile of this map.
    *
    * @return max Y.
    */
    t_word get_max_y();
    /**
    * @ brief Get the size of each sector in this map.
    *
    * @return size
    */
    t_ubyte get_sector_size();
    /**
    * @ brief Get the count of sectors for this map.
    *
    * @return count
    */
    t_ubyte get_sector_count();
    /**
    * @brief Get the map file used for this Map.
    *
    * @return file_id;
    */
    t_ubyte get_file_id();
    /**
    * @brief Get the count of UOMapBlock for this map.
    *
    * @return the count;
    */
    t_udword get_block_count();
    /**
    * @brief Checks if the given coords are valid.
    *
    * @param x coord X
    * @param y coord Y
    * @return if coords are valid.
    */
    bool is_valid_p(t_word x, t_word y);
};

#endif // __TORUS_GAME_MAP_H