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


void Server::init()
{
}

void Server::shutdown()
{
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

void Server::add_char(Char * character) {
    ADDTOCALLSTACK();
    _chars.add(character->get_uid(), character);
}

void Server::add_item(Item * item) {
    ADDTOCALLSTACK();
    _items.add(item->get_uid(), item);
}

Artifact * Server::get_artifact(Uid& uid) {
    ADDTOCALLSTACK();
    Artifact* art = nullptr;
    if (uid.is_char())
    {
        art = _chars.get(uid);
    }
    else if (uid.is_item())
    {
        art = _items.get(uid);
    }
    return art;
}

Char* Server::get_char(Uid& uid)
{
    return _chars.get(uid);
}

Item* Server::get_item(Uid& uid)
{
    return _items.get(uid);
}

void Server::del_char(Char * chr) {
    ADDTOCALLSTACK();
    _chars.delete_later(chr->get_uid());
}

void Server::del_item(Item * item) {
    ADDTOCALLSTACK();
    _items.delete_later(item->get_uid());
}

void Server::del_artifact(Artifact * art) {
    ADDTOCALLSTACK();
    if (art->get_uid().is_char())
    {
        _chars.delete_later(art->get_uid());
    }
    else if (art->get_uid().is_item())
    {
        _items.delete_later(art->get_uid());
    }
}

void Server::tick() {
    ADDTOCALLSTACK();
    //TODO TICKS
    _serv_time += _tick_period;
}
