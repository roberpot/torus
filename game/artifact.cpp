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
#include <string>
#include "../debug/debug.h"
#include "../debug/callstack.h"
#include "../core/torus.h"
#include "../db/db_manager.h"
#include "artifact.h"
#include "char.h"
#include "item.h"
#include "uo_files/map_list.h"
#include "server.h"

Artifact::Artifact(t_udword uid){
    ADDTOCALLSTACK();
    if ((uid &~(UID_ITEM | UID_RESOURCE)) == UID_CLEAR) {
        free_uid();
        set_uid_type(uid);
    }
    else {
        set_uid(uid);
    }
    _flags = 0;
    _color = 0;
}

t_udword Artifact::get_uid() {
    return Uid::get_uid();
}

Artifact::~Artifact(){
    ADDTOCALLSTACK();
    free_uid();
}

Char * Artifact::get_char() {
    ADDTOCALLSTACK();
    Char *pchar = static_cast<Char*>(this);
    if (!pchar)
        return nullptr;
    return pchar;
}

Item * Artifact::get_item() {
    ADDTOCALLSTACK();
    Item *pitem = static_cast<Item*>(this);
    if (!pitem)
        return nullptr;
    return pitem;
}

std::string Artifact::get_name(){
    ADDTOCALLSTACK();
    return _name;
}

void Artifact::set_name(std::string name){
    ADDTOCALLSTACK();
    // TODOTRIGGER: @Rename
    _name = name;
}

void Artifact::move_to(t_word destX, t_word destY){
    ADDTOCALLSTACK();
    if (!can_move_to_coord(destX, destY)) {
        DEBUG_ERROR("Trying to move 0x" << std::hex << get_uid() << "to a non-valid dest '" << destX << ", " << destY << ".");
    }
    /*bool updateMapCache = false;    // Blocking items updates the map cache to improve the speed of walking checks.
    if (is_item()) {
        Item *pitem = get_item();
        if (pitem && has_flag(UFLAG1_BLOCK)) {
            updateMapCache = true;
        }
    }
    if (updateMapCache) {
        maplist.get_map(map).get_map_point(x, y).del_item(); // remove item from current position.
    }*/
    x = destX;
    y = destY;
    /*if (updateMapCache) {
        maplist.get_map(map).get_map_point(x, y).add_item(); // adding item to the new position.
    }*/
}

void Artifact::set_z(t_byte destZ) {
    ADDTOCALLSTACK();
    if (!can_move_to_z(destZ)) {
        DEBUG_ERROR("Trying to move 0x" << std::hex << get_uid() << " out of limits (" << destZ << "), avoiding it.");
        return;
    }
    z = destZ;
}

void Artifact::set_map(t_ubyte destMap){
    ADDTOCALLSTACK();
    if (!can_move_to_map(destMap)) {
        DEBUG_ERROR("Trying to move 0x" << std::hex << get_uid() << " to map out of limits, avoiding it.");
        return;
    }
    map = destMap;
}

void Artifact::set_pos(t_word destX, t_word destY, t_byte destZ, t_ubyte destMap){
    ADDTOCALLSTACK();
    move_to(destX, destY);
    set_z(destZ);
    set_map(destMap);
}

Pos Artifact::get_pos() {
    return _pos;
}

t_uword Artifact::get_distance(Artifact * target) {
    ADDTOCALLSTACK();
    return get_distance(target->get_pos());
}

t_uword Artifact::get_distance(Pos target) {
    ADDTOCALLSTACK();
    return (_pos.x > target.x ? _pos.x - target.x : target.x - _pos.x) + (_pos.y > target.y ? _pos.y - target.y : target.y - _pos.y);
}

bool Artifact::has_flag(t_udword flag){
    ADDTOCALLSTACK();
    return _flags & flag;
}

void Artifact::set_flag(t_udword flag){
    ADDTOCALLSTACK();
    _flags |= flag;
}

void Artifact::unset_flag(t_udword flag){
    ADDTOCALLSTACK();
    _flags &= ~flag;
}

void Artifact::switch_flag(t_udword flag){
    ADDTOCALLSTACK();
    _flags ^= flag;
}

t_udword Artifact::get_flags() {
    return _flags;
}

void Artifact::set_color(t_uword color) {
    ADDTOCALLSTACK();
    _color = color;
}

t_uword Artifact::get_color() {
    ADDTOCALLSTACK();
    return _color;
}

t_uqword Artifact::get_timer() {
    ADDTOCALLSTACK();
    t_uqword diff = 0;
    t_uqword curtime = server.get_serv_time();
    if (_timer > curtime)
        diff = _timer - curtime;
    return diff;
}

void Artifact::set_timer(t_uqword ticks){
    ADDTOCALLSTACK();
    _timer = server.get_serv_time() + ticks;
}

bool Pos::can_move_to_coord(t_word destX, t_word destY){
    ADDTOCALLSTACK();
    if (destX < 0 || destX > maplist.get_map(map)->get_max_x()) {
        return false;
    }
    if (destY < 0 || destY > maplist.get_map(map)->get_max_y()) {
        return false;
    }
    /*
    if (maplist[map].get_flags() & MAPFLAGS_BLOCK){
        return false;    // TODO: Read map to set flags and create internal flags to do checks against them
    }
    */
    return true;
}

bool Pos::can_move_to_z(t_byte destZ){
    ADDTOCALLSTACK();
    if (destZ < CHAR_MIN || destZ > TBYTE_MAX) {
        return false;
    }
    return true;
}

bool Pos::can_move_to_map(t_ubyte destMap){
    ADDTOCALLSTACK();
    if (destMap > TUBYTE_MAX) {
        return false;
    }
    return true;
}

bool Pos::can_move_to(t_word destX, t_word destY, t_byte destZ, t_ubyte destMap) {
    return can_move_to_coord(destX, destY) && can_move_to_z(destZ) && can_move_to_map(destMap);
}
