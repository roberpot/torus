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

#include "uo_file_manager.h"
#include <sstream>
#include "../../debug/debug.h"
#include "../../debug/callstack.h"
#include "../../core/config.h"

UOFileManager uofilemgr;

UOFileManager::UOFileManager() {
}

UOFileManager::~UOFileManager() {
}

void UOFileManager::read_config() {
    ADDTOCALLSTACK();
    // Setup base maps
    for (size_t i = 0; i < toruscfg.map_files.size(); i++) {
        MapFile mapfile = toruscfg.map_files[i];
        Map *map = new Map();
        std::stringstream file;
        file << toruscfg.file_path << mapfile.filename;
        if (map->create(mapfile.x, mapfile.y, mapfile.sector_size, mapfile.id, file.str().c_str())) {
            basemaps[mapfile.id] = map;
        }
        else {
            DEBUG_ERROR("Error reading map file: " << file.str().c_str());
            delete map;
        }
    }
    // Create virtual maps
    for (size_t i = 0; i < toruscfg.maps.size(); i++) {
        std::pair<t_ubyte, t_ubyte> virtualmap = toruscfg.maps[i];
        Map *filemap = basemaps[virtualmap.second];
        if (filemap && filemap->get_file_id() == virtualmap.second) {  // Checking if there is
            maplist.add_map(virtualmap);
        }
        else {
            DEBUG_ERROR("Adding to virtual map '" << (int)virtualmap.first << "' the undefined base map '" << (int)virtualmap.second << "'.");
        }
    }
}

bool UOFileManager::init() {
    ADDTOCALLSTACK();
    read_config();

    t_ubyte mapcount = 0;
    for (size_t i = 0; i < basemaps.size(); i++) {
        if (basemaps[i]) {
            basemaps[i]->init();
            mapcount++;
        }
    }
    if (mapcount == 0){
        DEBUG_ERROR("No valid maps found.");
        return false;
    }
    //TODO add staticsN.mul load based upon base maps.

    return true;
}