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

#include <game/uo_files/map_list.h>
#include <debug_support/debug.h>
#include <debug_support/callstack.h>
#include <game/uo_files/uo_file_manager.h>

MapList maplist;

MapList::MapList()
{
}

MapList::~MapList() {
    for (int i = 0; i < _list.size(); ++i)
    {
        delete _list[i];
    }
}

Map *MapList::get_map(t_ubyte id) {
    ADDTOCALLSTACK();
    Map *map = nullptr;
    if (_list.size() > id)
    {
        map = _list[id];
    }
    
    return map;
}

t_ubyte MapList::get_map_count() {
    ADDTOCALLSTACK();
    return (t_ubyte)_list.size();
}

void MapList::add_map(t_ubyte index, Map* map){
    ADDTOCALLSTACK();
    if (get_map(index))
    {
        DEBUG_ERROR("MapList:: Trying to add already existing map" << index << ".\n");
    }
    else
    {
        _list[index] = map;
    }
}

void MapList::init()
{
    Map* map = new Map();   //TODO: Read from ini.
    map->create(0, 7168, 4096, 64, 0, "map0.mul");
    add_map(0, map);
}
