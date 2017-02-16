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

#include "map_list.h"
#include "../../debug/debug.h"
#include "../../debug/callstack.h"
#include "uo_file_manager.h"

MapList maplist;

MapList::~MapList() {
}

Map *MapList::get_map(t_ubyte id) {
    ADDTOCALLSTACK();
    return (*this)[id];
}

t_ubyte MapList::get_map_count() {
    ADDTOCALLSTACK();
    return (t_ubyte)(*this).size();
}

void MapList::add_map(std::pair<t_ubyte, t_ubyte> map){
    ADDTOCALLSTACK();
}
