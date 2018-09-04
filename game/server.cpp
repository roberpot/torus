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

#include "../library/system_headers.h"
#include "server.h"
#include "char.h"
#include "item.h"
#include "artifact.h"
#include "../core/config.h"

Server server;

Server::Server() {
    ADDTOCALLSTACK();
    _serv_time = 0;
    _tick_period = toruscfg.tick_duration;
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
    return true;
}

void Server::load_all() {
    ADDTOCALLSTACK();
}

void Server::save_all() {
    ADDTOCALLSTACK();
}

void Server::add_char(Char * chr) {
    ADDTOCALLSTACK();
    if (_artifact_list[chr->get_uid()]) {
        DEBUG_ERROR("Trying to add char with uid '" << chr->get_uid() << "' when there is already a char with this uid.");
        chr->remove();
    }
    else {
        _artifact_list[chr->get_uid()] = chr;
    }
}

void Server::add_item(Item * item) {
    ADDTOCALLSTACK();
    if (_artifact_list[item->get_uid()]) {
        DEBUG_ERROR("Trying to add char with uid '" << item->get_uid() << "' when there is already a char with this uid.");
        item->remove();
    }
    else {
        _artifact_list[item->get_uid()] = item;
    }
}

Artifact * Server::get_artifact(t_udword uid) {
    ADDTOCALLSTACK();
    Artifact * art = _artifact_list[uid];
    if (art)
        return art;
    return NULL;
}

void Server::del_char(Char * chr) {
    ADDTOCALLSTACK();
    _gclist.push_back(chr);
}

void Server::del_item(Item * item) {
    ADDTOCALLSTACK();
    _gclist.push_back(item);
}

void Server::del_artifact(Artifact * art) {
    ADDTOCALLSTACK();
    t_udword total = (t_udword)_artifact_list.size();
    if (total) {
        for (t_udword i = 0; i < total; ++i) {
            Artifact *tmp = _artifact_list[i];
            if (tmp == art) {
                delete tmp;
                _artifact_list.erase(i);
            }
        }
    }
}

void Server::tick() {
    ADDTOCALLSTACK();
    if (_gclist.size()) {
        while (_gclist.size()) {
            del_artifact(_gclist[0]);
        }
    }
    _serv_time += _tick_period;
}
