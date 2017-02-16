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

#ifndef _TORUS_GAME_ARTIFACT_H
#define _TORUS_GAME_ARTIFACT_H

#include "../core/types.h"
#include "uid.h"
#include <combaseapi.h>

struct Pos {
    t_word x = 1;
    t_word y = 1;
    t_byte z = 0;
    t_ubyte map = 0;
    bool can_move_to_coord(t_word destX, t_word destY);
    bool can_move_to_z(t_byte destZ);
    bool can_move_to_map(t_ubyte destMap);
    bool can_move_to(t_word destX, t_word destY, t_byte destZ, t_ubyte destMap);
};

class Char;
class Item;

class Artifact : protected Uid, public Pos {
protected:
    //Uid _uid;
    virtual ~Artifact();
    Artifact(t_udword uid);
    Char *get_char();
    Item *get_item();

    //Name
private:
    const t_byte *_name;
public:
    const t_byte *get_name();
    void set_name(const t_byte *name);

    //Pos
private:
    Pos _pos;
public:
    void move_to(t_word destX, t_word destY);
    void set_z(t_byte destZ);
    void set_map(t_ubyte destMap);
    void set_pos(t_word destX, t_word destY, t_byte destZ, t_ubyte destMap);
    virtual bool can_move() = 0;

    //Flags
private:
    t_uword _flags;
public:
    bool has_flag(t_udword flag);
    void set_flag(t_udword flag);
    void unset_flag(t_udword flag);
    void switch_flag(t_udword flag);
    t_udword get_flags();

private:
    t_udword _color;
public:
    void set_color(t_udword color);
    t_udword get_color();
};

#endif //_TORUS_GAME_ARTIFACT_H