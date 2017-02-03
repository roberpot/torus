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

#ifndef _TORUS_GAME_IDX_ENTRY_H
#define _TORUS_GAME_IDX_ENTRY_H

#include "../../core/types.h"

struct IdxEntry {
    t_word lookup = 0;  ///< first byte of the required data in the .mul file
    t_word lenght = 0;  ///< lenght of the data.
    t_word extra = 0;   ///< some files use this value.
};
#endif //_TORUS_GAME_IDX_ENTRY_H