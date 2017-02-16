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

#ifndef _TORUS_GAME_UO_FILE_MANAGER_H
#define _TORUS_GAME_UO_FILE_MANAGER_H

#include "../../core/types.h"
#include <vector>
#include "map.h"
#include "map_list.h"

extern class UOFileManager{
public:
    UOFileManager();
    ~UOFileManager();

private:
    std::map<t_ubyte, Map*> basemaps;
    MapList maplist;
public:
    void read_config();
    bool init();

} uofilemgr;

#endif //_TORUS_GAME_UO_FILE_MANAGER_H