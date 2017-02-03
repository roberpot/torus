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

#include "uo_file_reader.h"
#include <sstream>
#include <cstdio>
#include "../../debug/debug.h"
#include "../../debug/callstack.h"
#include "../../core/config.h"
#include "map_list.h"
#include "map.h"
#include "map_point.h"

UOFileReader uofilereader;

UOFileReader::UOFileReader() {
    ADDTOCALLSTACK();
}

UOFileReader::~UOFileReader() {
    ADDTOCALLSTACK();
}

bool UOFileReader::_open(const t_byte *file, t_udword flags) {
    ADDTOCALLSTACK();
    if (is_open()) {
        DEBUG_ERROR("Trying to open a file when another one is already opened.");
        return false;
    }
    std::stringstream final_file;
    final_file << toruscfg.file_path << file;
    open(final_file.str().c_str(), flags);
    return is_open();
}

bool UOFileReader::_close() {
    ADDTOCALLSTACK();
    close();
    return true;
}

bool UOFileReader::_seek(t_uqword index, t_ubyte from) {
    ADDTOCALLSTACK();
    seekg(index, from);
    if (!tellg() == index) {
        return false;
    }
    return true;
}

void UOFileReader::get_idx(IdxEntry &idx) {
    ADDTOCALLSTACK();
    idx.lookup = get_word();
    idx.lenght = get_word();
    idx.extra = get_word();
}

t_byte UOFileReader::get_byte() {
    t_byte v = 0;
    read((char *)&v, sizeof(t_byte));
    return v;
}

t_ubyte UOFileReader::get_ubyte() {
    t_ubyte v = 0;
    read((char *)&v, sizeof(t_ubyte));
    return v;
}

t_word UOFileReader::get_word() {
    t_word v = 0;
    read((char *)&v, sizeof(t_word));
    return v;
}

t_uword UOFileReader::get_uword() {
    t_uword v = 0;
    read((char *)&v, sizeof(t_uword));
    return v;
}

t_dword UOFileReader::get_dword() {
    t_dword v = 0;
    read((char *)&v, sizeof(t_dword));
    return v;
}

t_udword UOFileReader::get_udword() {
    t_udword v = 0;
    read((char *)&v, sizeof(t_udword));
    return v;
}