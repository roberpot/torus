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

#ifndef _TORUS_GAME_MAP_H
#define _TORUS_GAME_MAP_H

#include "../../core/types.h"
#include "map_block.h"

class Map {
private:
    t_uword _x = 0;     // max X
    t_uword _y = 0;     // max Y
    t_ubyte _fileid = 255;    // map file in which this map is based on (255 = bad id).
    UOMapBlock *_blocks;
    bool _isuop;
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
    */
    void create(t_word x, t_word y, t_ubyte file_id);
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
    t_uword get_block_count();
    /**
    * @brief Checks if the given coords are valid.
    *
    * @param x coord X
    * @param y coord Y
    * @return if coords are valid.
    */
    bool is_valid_p(t_word x, t_word y);
    /**
    * @brief Fills the Map with it's map file's data.
    *
    * @return success or fail;
    */
    bool read_from_file();
};

#endif //_TORUS_GAME_MAP_H