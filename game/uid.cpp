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

#include "uid.h"
#include "../shell.h"
#include "../debug/callstack.h"

t_udword Uid::highest_uid = 0;

Uid::Uid(){
    ADDTOCALLSTACK();
    _uid = UID_UNUSED;
    find_new_uid();
}

Uid::Uid(t_udword uid){
    ADDTOCALLSTACK();
    set_uid(uid);
}

Uid::~Uid(){
    ADDTOCALLSTACK();
    free_uid();
}

void Uid::set_uid(t_udword uid) {
    ADDTOCALLSTACK();
    _uid = uid;
    if (highest_uid < get_uid_base())
        highest_uid = get_uid_base();
}

void Uid::find_new_uid(){
    ADDTOCALLSTACK();
    // TODO: DB Table with free uids, query it and use the first free value or create a new index
    set_uid(highest_uid);
}

void Uid::free_uid(){
    ADDTOCALLSTACK();
    // TODO: DB Table to store free uids.
}

t_udword Uid::get_uid(){
    ADDTOCALLSTACK();
    return _uid;
}

t_udword Uid::get_uid_base(){
    ADDTOCALLSTACK();
    return _uid &~(UID_ITEM | UID_RESOURCE);
}

void Uid::set_uid_type(t_udword mask) {
    ADDTOCALLSTACK();
    _uid |= mask;
}

bool Uid::is_item(){
    ADDTOCALLSTACK();
    return _uid & UID_ITEM;
}

bool Uid::is_resource(){
    ADDTOCALLSTACK();
    return _uid & UID_RESOURCE;
}

bool Uid::is_char() {
    ADDTOCALLSTACK();
    return !is_item() && !is_resource();
}

bool Uid::is_valid(){
    ADDTOCALLSTACK();
    return !(_uid &UID_UNUSED);
}