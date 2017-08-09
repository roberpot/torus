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

#include "../library/types.h"
#include "../library/map.h"
#include "../library/vector.h"
#include "uid.h"

class Char;
class Item;
class Artifact;

extern class Server {
    t_uqword _serv_time; ///< Server time in ticks.
    t_uword _tick_period;
public:
    Server();
    ~Server();
    t_uqword get_serv_time();
    bool check();
    void load_all();
    void save_all();
    ttl::tsdynamicmap<t_udword, Artifact*> _artifact_list;
    void add_char(Char *chr);
    void add_item(Item *item);
    Artifact * get_artifact(t_udword uid);
    ttl::tsvector<Artifact*> _gclist;
    void del_char(Char *chr);
    void del_item(Item *item);
    void del_artifact(Artifact *art);
    void tick();

} server;

#endif // __TORUS_GAME_SERVER_H_
