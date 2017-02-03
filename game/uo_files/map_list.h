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

#ifndef _TORUS_GAME_MAP_LIST_H
#define _TORUS_GAME_MAP_LIST_H

#include "../../core/types.h"
#include <map>
#include "map.h"

extern class MapList : private std::map < t_ubyte, Map>{
public:
    MapList() {}
    ~MapList();
    /**
    * @brief check if the map_list was correctly initialized.
    */
    bool init();
    /**
    * @brief gets the Map with the given id.
    *
    * Searchs for a valid _map[id] with _mapid = id, if that _map[]._mapid and id doesn't match it searchs with a for loop for a match.
    * @param id The ingame ID of this map.
    * @return The Map with this id.
    */
    Map &get_map(t_ubyte id);

    /**
    * @brief Get the total count of maps.
    *
    * @return N of maps.
    */
    t_ubyte get_map_count();

    void add_map(t_uword x, t_uword y, t_ubyte id, t_ubyte file_id);
} maplist;

#endif //_TORUS_GAME_MAP_LIST_H