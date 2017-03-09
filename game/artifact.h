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

#ifndef __TORUS_GAME_ARTIFACT_H
#define __TORUS_GAME_ARTIFACT_H

#include "../db/torus_db_object.h"
#include "../library/types.h"
#include "uid.h"

#define TABLENAME_ARTIFACTS "artifacts"
#define COLNAME_ARTIFACTS_UID "uid"
#define COLNAME_ARTIFACTS_NAME "name"
#define COLNAME_ARTIFACTS_COLOR "color"
#define COLNAME_ARTIFACTS_FLAGS "flags"
#define COLNAME_ARTIFACTS_POSX "pos.x"
#define COLNAME_ARTIFACTS_POSY "pos.y"
#define COLNAME_ARTIFACTS_POSZ "pos.z"
#define COLNAME_ARTIFACTS_POSM "pos.m"

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

class Artifact : protected Uid, protected Pos, public TDBObject {
public:
    t_udword get_uid();
protected:
    //Uid _uid;
    virtual ~Artifact();
    Artifact(t_udword uid);
    Char *get_char();
    Item *get_item();

public:
    bool db_load(pqxx::result::const_iterator r);
    bool db_save();
    void mark_db_update();
    void mark_db_delete();
    //Name
private:
    std::string _name;
public:
    std::string get_name();
    void set_name(std::string name);

    //Pos
private:
    Pos _pos;
public:
    void move_to(t_word destX, t_word destY);
    void set_z(t_byte destZ);
    void set_map(t_ubyte destMap);
    void set_pos(t_word destX, t_word destY, t_byte destZ, t_ubyte destMap);
    Pos get_pos();
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

private:
    t_uqword _timer;
public:
    /**
    * @brief Gets the current timer for this artifact.
    *
    * @return the timer.
    */
    t_uqword get_timer();
    /**
    * @brief Sets a new timer for this artifact.
    *
    * @param ticks The new time in ticks.
    */
    void set_timer(t_uqword ticks);
};

#endif // __TORUS_GAME_ARTIFACT_H
