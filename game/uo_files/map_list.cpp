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
#include <cstddef>
#include <cstring>
#include "../../debug/debug.h"
#include "../../debug/callstack.h"


MapList::~MapList() {
    ADDTOCALLSTACK();
}

bool MapList::init() {
    ADDTOCALLSTACK();
    t_ubyte valid_maps = 0;
    iterator itb = begin(), ite = end();
    while (itb != ite) {
        if ((*itb).second.is_valid())
            valid_maps++;
    }
    if (valid_maps == 0) {
        return false;
    }
    return true;
}

Map &MapList::get_map(t_ubyte id) {
    ADDTOCALLSTACK();
    return (*this)[id];
}

t_ubyte MapList::get_map_count() {
    ADDTOCALLSTACK();
    return (t_ubyte)(*this).size();
}

void MapList::add_map(t_uword x, t_uword y, t_ubyte id, t_ubyte file_id) {
    if ((*this)[id].is_valid()) {
        DEBUG_ERROR("Trying to add an already existing Map: " << x << ", " << y << ", " << id << ", " << file_id);
        return;
    }
    (*this)[id].create(x, y, file_id);
}
