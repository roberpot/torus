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

#include <library/system_headers.h>
#include <game/server.h>
#include <game/char.h>
#include <game/item.h>
#include <game/artifact.h>
#include <core/config.h>

Server server;

Server::Server() {
    ADDTOCALLSTACK();
    _serv_time = 0;
    _tick_period = toruscfg.tick_duration;
}

Server::~Server() {
    ADDTOCALLSTACK();
}

uqword_t Server::get_serv_time() {
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
    _chars.add(chr->get_uid(), chr);
    //TODO WOC
}

void Server::add_item(Item * item) {
    ADDTOCALLSTACK();
    //TODO WOC
}

Artifact * Server::get_artifact(const Uid& uid) {
    ADDTOCALLSTACK();
    //TODO WOC
    return nullptr;
}

Char* Server::get_char(const Uid& uid)
{
    Char *character = nullptr;
    //TODO WOC
    return character;
}

Item* Server::get_item(const Uid& uid)
{
    //TODO WOC
    return nullptr;
}

void Server::del_char(Char * chr) {
    ADDTOCALLSTACK();
    //TODO WOC
}

void Server::del_item(Item * item) {
    ADDTOCALLSTACK();
    //TODO WOC
}

void Server::del_artifact(Artifact * art) {
    ADDTOCALLSTACK();
    //TODO WOC
}

void Server::tick() {
    ADDTOCALLSTACK();
    //TODO TICKS
    _serv_time += _tick_period;
}
