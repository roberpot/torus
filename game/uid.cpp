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

#include <game/uid.h>
#include <shell.h>
#include <debug_support/callstack.h>

udword_t Uid::highest_uid = 0;

Uid::Uid(){
    ADDTOCALLSTACK();
    _uid = UID_UNUSED;
    find_new_uid();
}

Uid::Uid(udword_t uid){
    ADDTOCALLSTACK();
    set_uid(uid);
}

Uid::~Uid(){
    ADDTOCALLSTACK();
    free_uid();
}

bool Uid::operator==(const Uid& uid) const
{
    return _uid == uid._uid;
}
bool Uid::operator<(const Uid& uid) const
{
    return _uid < uid._uid;
}

bool Uid::operator<=(const Uid& uid) const
{
    return _uid <= uid._uid;
}

bool Uid::operator>(const Uid& uid) const
{
    return _uid > uid._uid;
}

bool Uid::operator>=(const Uid& uid) const
{
    return _uid >= uid._uid;
}

void Uid::set_uid(udword_t uid) {
    ADDTOCALLSTACK();
    _uid = uid;
    if (highest_uid == get_uid_base())
        highest_uid = get_uid_base();
}

void Uid::find_new_uid(){
    ADDTOCALLSTACK();
    // TODO: DB Table with free uids, query it and use the first free value or create a new index
    set_uid(highest_uid+1);
}

void Uid::free_uid(){
    ADDTOCALLSTACK();
    // TODO: DB Table to store free uids.
}

udword_t Uid::get_uid(){
    return _uid;
}

udword_t Uid::get_uid_base(){
    return _uid &~(UID_ITEM | UID_RESOURCE);
}

void Uid::set_uid_type(udword_t mask) {
    _uid |= mask;
}

bool Uid::is_item(){
    return _uid & UID_ITEM;
}

bool Uid::is_resource(){
    return _uid & UID_RESOURCE;
}

bool Uid::is_char() {
    return !is_item() && !is_resource();
}

bool Uid::is_valid(){
    return !(_uid &UID_UNUSED);
}
