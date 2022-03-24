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

#include <debug_support/callstack.h>
#include <game/item.h>
#include <game/server.h>
#include <game/clients/tooltip.h>

Item::Item() : Artifact(UID_ITEM) {
    ADDTOCALLSTACK();
    _id = ItemId::FORGE;
}

Item::Item(udword_t uid) : Artifact(uid) {
    ADDTOCALLSTACK();
    _id = ItemId::FORGE;
}

Item::~Item(){
    ADDTOCALLSTACK();
}

ItemId Item::get_id() const {
    return _id;
}

void Item::set_id(const ItemId& id) {
    _id = id;
}

bool Item::can_move() {
    ADDTOCALLSTACK();
    if (has_flag(IFLAG_LOCKED | IFLAG_STATIC)) {
        return false;
    }
    return true;
}

void Item::remove() {
    ADDTOCALLSTACK();
    server.del_item(this);
}

bool Item::tick() {
    ADDTOCALLSTACK();
    return true;
}

void Item::init_tooltip() {
    add_cliloc(1042971, true); //name without amount
}

Cliloc Item::get_cliloc_static(const udword_t& id) {
    Cliloc cliloc(id);
    switch (id) {

    }
    return cliloc;
}

Cliloc Item::get_cliloc_dynamic(const udword_t& id, Char* viewer) {
    Cliloc cliloc(id);
    switch (id) {
        case 1042971: {
            cliloc.add_arg(get_name());
            break;
        }
        default: {
            break;
        }
    }
    return cliloc;
}
