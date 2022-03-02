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

#ifndef __TORUS_GAME_UO_FILE_MANAGER_H
#define __TORUS_GAME_UO_FILE_MANAGER_H

#include <vector>
#include <library/types.h>
#include <game/uo_files/map.h>
#include <game/uo_files/map_list.h>

extern class UOFileManager{
public:
private:
    std::map<t_ubyte, Map*> basemaps;
public:
    void read_config();

    /**
     * @brief Initializer for extern class.
     */
    bool init();
    /**
     * @brief Destroyer for extern class.
     */
    void shutdown();

} uofilemgr;

#endif // __TORUS_GAME_UO_FILE_MANAGER_H
