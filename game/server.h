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

#ifndef __TORUS_GAME_SERVER_H_
#define __TORUS_GAME_SERVER_H_

#include <library/types.h>
#include <library/map.h>
#include <library/vector.h>
#include <game/uid.h>
#include <game/word_object_container.h>

class Char;
class Item;
class Artifact;

extern class Server {
    uqword_t _serv_time; ///< Server time in ticks.
    uword_t _tick_period;

    WorldObjectContainer<Char*> _chars;
    WorldObjectContainer<Item*> _items;
public:
    /**
     * @brief Initializer for extern class.
     */
    void init();
    /**
     * @brief Destroyer for extern class.
     */
    void shutdown();
    uqword_t get_serv_time();
    bool check();
    void load_all();
    void save_all();
    void add_char(Char* chr);
    void add_item(Item *item);
    Artifact * get_artifact(Uid& uid);
    Char *get_char(Uid& uid);
    Item *get_item(Uid& uid);
    void del_char(Char *chr);
    void del_item(Item *item);
    void del_artifact(Artifact *art);
    void tick();

} server;

#endif // __TORUS_GAME_SERVER_H_
