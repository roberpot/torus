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

#include <cstring>

#include <game/uo_files/map.h>
#include <debug_support/callstack.h>
#include <debug_support/debug.h>

Map::Map(){
    _index = TUWORD_MAX;
    _x = 0;
    _y = 0;
    _fileid = 0;
    _sector_size = 64;
    _blocks = nullptr;
    _filename = "map0.mul";
    _is_valid = false;
}

Map::~Map(){
    if (_blocks)
    {
        delete _blocks;
    }
}

bool Map::create(uword_t index, uword_t x, uword_t y, t_ubyte ss, t_ubyte file_id, std::string filename)
{
    ADDTOCALLSTACK();
    /*std::ifstream filetest(filename.c_str());
    if (!filetest.is_open()) {
        return false;
    }
    filetest.close();*/
    _index = index;
    _x = x;
    _y = y;
    _fileid = file_id;
    _sector_size = ss;
    _filename = filename;

    _blocks = new UOMapBlock[get_block_count()];
    memset((char*)_blocks, 0, sizeof(UOMapBlock) * get_block_count());
    return true;
}

bool Map::is_valid() {
    ADDTOCALLSTACK();
    return _is_valid;
}

word_t Map::get_max_x(){
    ADDTOCALLSTACK();
    return _x;
}

word_t Map::get_max_y(){
    ADDTOCALLSTACK();
    return _y;
}

t_ubyte Map::get_sector_size() {
    ADDTOCALLSTACK();
    return _sector_size;
}

t_ubyte Map::get_sector_count() {
    ADDTOCALLSTACK();
    return (t_ubyte)(_x * _y) / get_sector_size();
}

t_ubyte Map::get_file_id() {
    ADDTOCALLSTACK();
    return _fileid;
}

udword_t Map::get_block_count() {
    ADDTOCALLSTACK();
    return (_x * _y) / UO_MAP_BLOCK_CELLS;
}

bool Map::is_valid_p(word_t x, word_t y){
    ADDTOCALLSTACK();
    if (x <= 0 || y <= 0
        || x > _x || y > _y)
        return false;
    return true;
}

bool Map::init(){
    ADDTOCALLSTACK();
    /*std::streamsize bytes_to_read = sizeof(UOMapBlock) * get_block_count();
    
    // File opening & checks
    std::ifstream mapfile(_filename.c_str(), std::ios_base::in | std::ios_base::binary);
    if (!mapfile.is_open()){
        DEBUG_ERROR("Couldn't open map file '" << _filename.c_str() << "'.");
        return false;
    }

    // Reading data...
    mapfile.read((char*)_blocks, bytes_to_read);
    _is_valid = mapfile.gcount() == bytes_to_read;

    // Done reading, closing ...
    mapfile.close();

    // Checking integrity of readed data.
    if (!_is_valid) {
        DEBUG_ERROR("Error reading map " << (int)get_file_id() << ", readed " << mapfile.gcount() << " bytes out of " << bytes_to_read << ".");
        return false;
    }*/
    return true;
}
