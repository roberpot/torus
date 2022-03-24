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

#include <string>

#include <library/types.h>
#include <game/coord_point.h>
#include <game/enums.h>
#include <game/uid.h>
#include <game/clients/tooltip.h>


class Char;
class Item;

class Artifact {
protected:
    Artifact(udword_t uid);
    Char *get_char();
    Item *get_item();
    virtual void remove() = 0;
    virtual bool tick() = 0;

public:
    virtual ~Artifact();
    //Name
private:
    std::string _name;
    Uid _uid;
public:
    std::string get_name();
    void set_name(std::string name);

    Uid& get_uid();

    //Pos
private:
    CoordPoint _position;
public:
    void move_to(word_t destX, word_t destY);
    void set_z(t_byte destZ);
    void set_map(t_ubyte destMap);
    void set_pos(word_t destX, word_t destY);
    void set_pos(word_t destX, word_t destY, t_byte destZ);
    void set_pos(word_t destX, word_t destY, t_byte destZ, t_ubyte destMap);
    void set_pos(const CoordPoint& p);
    CoordPoint get_pos();
    uword_t get_distance(Artifact *target);
    uword_t get_distance(CoordPoint target);
    virtual bool can_move() = 0;

    //Flags
private:
    udword_t _flags;
public:
    bool has_flag(udword_t flag);
    void set_flag(udword_t flag);
    void unset_flag(udword_t flag);
    void switch_flag(udword_t flag);
    udword_t get_flags();

private:
    uword_t _color;
public:
    void set_color(uword_t color);
    uword_t get_color();

private:
    uqword_t _timer;
public:
    /**
    * @brief Gets the current timer for this artifact.
    *
    * @return the timer.
    */
    uqword_t get_timer();
    /**
    * @brief Sets a new timer for this artifact.
    *
    * @param ticks The new time in ticks.
    */
    void set_timer(uqword_t ticks);

//Dir
private:
    Dir _dir;
public:
    void set_dir(Dir dir);
    Dir get_dir();

//Tooltips
private:
    Tooltip _tooltip;
public:
    const Tooltip& get_tooltip();
    void add_cliloc(udword_t id, bool dynamic = false);
    virtual void init_tooltip() = 0;
    virtual Cliloc get_cliloc_static(const udword_t& id) = 0;
    virtual Cliloc get_cliloc_dynamic(const udword_t& id, Char* viewer) = 0;
};

#endif // __TORUS_GAME_ARTIFACT_H
