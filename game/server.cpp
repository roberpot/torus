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

#include "server.h"
#include "../db/db_manager.h"

Server server;

Server::Server() {
    ADDTOCALLSTACK();
    _serv_time = 0;
}

Server::~Server() {
    ADDTOCALLSTACK();
}

t_uqword Server::get_serv_time() {
    ADDTOCALLSTACK();
    return _serv_time;
}

bool Server::check() {
    ADDTOCALLSTACK();
    std::stringstream query;
    t_udword fails = 0;
    query << "SELECT " << COLNAME_SERVER_TIME << " from " << TABLENAME_SERVER << " LIMIT 1";
    if (!torusdb.exec(query.str())) {
        fails++;
    }
    if (fails > 0)
        return false;
    return true;
}

void Server::load_all() {
    ADDTOCALLSTACK();
}

void Server::save_all() {
    ADDTOCALLSTACK();
}
