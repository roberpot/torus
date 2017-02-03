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

#include "map.h"
#include <iostream>
#include <cstring>
#include "../../debug/callstack.h"
#include "../../debug/debug.h"
#include "uo_file_reader.h"

Map::Map(){
    _x = 0;
    _y = 0;
    _fileid = 0;
    _is_valid = false;
    _isuop = false;
}

Map::~Map(){
    ADDTOCALLSTACK();
}

void Map::create(t_word x, t_word y, t_ubyte file_id){
    ADDTOCALLSTACK();
    _x = x;
    _y = y;
    _fileid = file_id;

    _blocks = new UOMapBlock[get_block_count()];
    memset(_blocks, 0, sizeof(UOMapBlock) * get_block_count());
    if (read_from_file())
        _is_valid = true;
}

bool Map::is_valid() {
    return _is_valid;
}

t_word Map::get_max_x(){
    ADDTOCALLSTACK();
    return _x;
}

t_word Map::get_max_y(){
    ADDTOCALLSTACK();
    return _y;
}

t_ubyte Map::get_file_id() {
    ADDTOCALLSTACK();
    return _fileid;
}

t_uword Map::get_block_count() {
    return (_x + _y) / UO_MAP_BLOCK_SIZE;
}

bool Map::is_valid_p(t_word x, t_word y){
    ADDTOCALLSTACK();
    if (x <= 0 || y <= 0
        || x > _x || y > _y)
        return false;
    return true;
}

bool Map::read_from_file(){
    ADDTOCALLSTACK();
    std::stringstream fileuop;
    fileuop << "map" << (int)_fileid << "LegacyUOP.mul";
    if (uofilereader._open(fileuop.str().c_str())) {   //try to read mapNLegacyMUL.uop
        _isuop = true;
    }
    else {
        std::stringstream filemul;
        filemul << "map" << (int)_fileid << ".mul";
        if (!uofilereader._open(filemul.str().c_str())) { // if no .uop was found, try with mapN.mul.
            DEBUG_ERROR("Couldn't open map files (either .mul or .uop).");
            return false;
        }
    }

    t_udword blocks_count = (_x / 8) * (_y / 8);

    for (t_udword block = 0; block < blocks_count; block++) {
        _blocks[block].init();
        uofilereader._seek(sizeof(UOMapBlock)*block, std::ifstream::beg);
        _blocks[block].header_lo = uofilereader.get_uword();
        _blocks[block].header_hi = uofilereader.get_uword();
        for (t_uword cell = 0; cell < 64; cell++) {
            _blocks[block].points[cell].terrain = uofilereader.get_word();
            _blocks[block].points[cell].z = uofilereader.get_byte();
        }
    }
    return true;
}
