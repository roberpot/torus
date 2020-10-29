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
#include <game/uid.h>
#include <game/server.h>

struct Pos {
    word_t x = 1;
    word_t y = 1;
    t_byte z = 0;
    t_ubyte map = 0;
    bool can_move_to_coord(word_t destX, word_t destY);
    bool can_move_to_z(t_byte destZ);
    bool can_move_to_map(t_ubyte destMap);
    bool can_move_to(word_t destX, word_t destY, t_byte destZ, t_ubyte destMap);
};

class Char;
class Item;

class Artifact : public Uid, public Pos {
public:
    udword_t get_uid();
protected:
    Artifact(udword_t uid);
    Char *get_char();
    Item *get_item();
    virtual void remove() = 0;
    virtual bool tick() = 0;

public:
    friend void Server::tick();
    virtual ~Artifact();
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
    void move_to(word_t destX, word_t destY);
    void set_z(t_byte destZ);
    void set_map(t_ubyte destMap);
    void set_pos(word_t destX, word_t destY, t_byte destZ, t_ubyte destMap);
    Pos get_pos();
    uword_t get_distance(Artifact *target);
    uword_t get_distance(Pos target);
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
};

#endif // __TORUS_GAME_ARTIFACT_H
