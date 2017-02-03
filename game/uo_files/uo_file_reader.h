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

#ifndef _TORUS_GAME_UO_FILE_READER_H
#define _TORUS_GAME_UO_FILE_READER_H

#include <fstream>
#include "../../core/types.h"
#include "map.h"
#include "idx_entry.h"


extern class UOFileReader : public std::ifstream {
public:
    UOFileReader();
    ~UOFileReader();

    /**
    * @brief Overrides seekg with custom checks.
    *
    * Checks if the index requested is within the file's size.
    * @param index Position to set the pointer at.
    * @param from Start reading from (std::ios::beg / end).
    * @return success or fail.
    */
    bool _seek(t_uqword index, t_ubyte from = std::ios::beg);
    /**
    * @brief Overrides open with custom checks.
    *
    * Checks if the is a file already open, also sets _size for fast later accesses.
    * @param file Name of the file to be opened.
    * @param flags Flags used to open the file.
    * @return success or fail.
    */
    bool _open(const t_byte *file, t_udword flags = std::ifstream::in | std::ifstream::binary);

    bool _close();
    /**
    * @brief Get the map/uop header into the given IdxEntry.
    *
    * Reads a .idx file at 'index' position to retrieve the header for the mul/uop files. (Pointer must be placed with _seek before).
    * @param idx the IdxEntry to insert data into.
    */
    void get_idx(IdxEntry &idx);

    /*
    * @brief Returns the next value of the requested type.
    */
    t_byte get_byte();
    /*
    * @brief Returns the next value of the requested type.
    */
    t_ubyte get_ubyte();
    /*
    * @brief Returns the next value of the requested type.
    */
    t_word get_word();
    /*
    * @brief Returns the next value of the requested type.
    */
    t_uword get_uword();
    /*
    * @brief Returns the next value of the requested type.
    */
    t_dword get_dword();
    /*
    * @brief Returns the next value of the requested type.
    */
    t_udword get_udword();

} uofilereader;

#endif //_TORUS_GAME_UO_FILE_READER_H