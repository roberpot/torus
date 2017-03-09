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

#include "../db/torus_db_table.h"
#include "../library/types.h"

#define TABLENAME_SERVER "server"
#define COLNAME_SERVER_TIME "time"

extern class Server : public TDBTable {
    t_uqword _serv_time; ///< Server time in ticks.
    Server();
    ~Server();
public:
    t_uqword get_serv_time();
    bool check();
    void load_all();
    void save_all();

} server;

#endif // __TORUS_GAME_SERVER_H_
