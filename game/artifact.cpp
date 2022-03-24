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

#include <string>
#include <library/system_headers.h>
#include <debug_support/debug.h>
#include <debug_support/callstack.h>
#include <core/torus.h>
#include <game/artifact.h>
#include <game/char.h>
#include <game/item.h>
#include <game/uo_files/map_list.h>
#include <game/server.h>

Artifact::Artifact(udword_t uid){
    ADDTOCALLSTACK();
    if ((uid &~(UID_ITEM | UID_RESOURCE)) == UID_CLEAR) {
        _uid.free_uid();
        _uid.set_uid_type(uid);
    }
    else {
        _uid.set_uid(uid);
    }
    _flags = 0;
    _color = 0;
    _dir = Dir::N;
}

Uid& Artifact::get_uid() {
    return _uid;
}

Artifact::~Artifact(){
   ADDTOCALLSTACK();
    _uid.free_uid();
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
    return _name;
}

void Artifact::set_name(std::string name){
    // TODOTRIGGER: @Rename
    _name = name;
}

void Artifact::move_to(word_t destX, word_t destY){
   ADDTOCALLSTACK();
    if (!_position.can_move_to_coord(destX, destY)) {
        //DEBUG_ERROR("Trying to move 0x" << std::hex << get_uid().get_uid() << "to a non-valid dest " << std::dec << destX << ", " << destY);
        //DEBUG_ERROR("Trying to move to a non-valid dest " << std::dec << destX << ", " << destY);
        DEBUG_ERROR("Trying to move to a non-valid dest");
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
    _position.set_point(destX, destY);
    /*if (updateMapCache) {
        maplist.get_map(map).get_map_point(x, y).add_item(); // adding item to the new position.
    }*/
}

void Artifact::set_z(t_byte destZ) {
   ADDTOCALLSTACK();
    if (!_position.can_move_to_z(destZ)) {
        DEBUG_ERROR("Trying to move 0x" << std::hex << get_uid().get_uid() << " out of limits (" << destZ << "), avoiding it.");
        return;
    }
    _position.set_z(destZ);
}

void Artifact::set_map(t_ubyte destMap){
   ADDTOCALLSTACK();
    if (!_position.can_move_to_map(destMap)) {
        DEBUG_ERROR("Trying to move 0x" << std::hex << get_uid().get_uid() << " to map out of limits, avoiding it.");
        return;
    }
    _position.set_map(destMap);
}

void Artifact::set_pos(word_t destX, word_t destY) {
    ADDTOCALLSTACK();
    move_to(destX, destY);
}

void Artifact::set_pos(word_t destX, word_t destY, t_byte destZ) {
    ADDTOCALLSTACK();
    move_to(destX, destY);
    set_z(destZ);
}

void Artifact::set_pos(word_t destX, word_t destY, t_byte destZ, t_ubyte destMap){
   ADDTOCALLSTACK();
    move_to(destX, destY);
    set_z(destZ);
    set_map(destMap);
}

void Artifact::set_pos(const CoordPoint& p) {
    ADDTOCALLSTACK();
    _position = p;
}

CoordPoint Artifact::get_pos() {
    return _position;
}

uword_t Artifact::get_distance(Artifact * target) {
    ADDTOCALLSTACK();
    return get_distance(target->get_pos());
}

uword_t Artifact::get_distance(CoordPoint target) {
   ADDTOCALLSTACK();
    return _position.get_distance(target);
}

bool Artifact::has_flag(udword_t flag){
   ADDTOCALLSTACK();
    return _flags & flag;
}

void Artifact::set_flag(udword_t flag){
   ADDTOCALLSTACK();
    _flags |= flag;
}

void Artifact::unset_flag(udword_t flag){
   ADDTOCALLSTACK();
    _flags &= ~flag;
}

void Artifact::switch_flag(udword_t flag){
   ADDTOCALLSTACK();
    _flags ^= flag;
}

udword_t Artifact::get_flags() {
    return _flags;
}

void Artifact::set_color(uword_t color) {
   ADDTOCALLSTACK();
    _color = color;
}

uword_t Artifact::get_color() {
   ADDTOCALLSTACK();
    return _color;
}

uqword_t Artifact::get_timer() {
   ADDTOCALLSTACK();
    uqword_t diff = 0;
    uqword_t curtime = server.get_serv_time();
    if (_timer > curtime)
        diff = _timer - curtime;
    return diff;
}

void Artifact::set_timer(uqword_t ticks){
   ADDTOCALLSTACK();
    _timer = server.get_serv_time() + ticks;
}


void Artifact::set_dir(Dir dir) {
    ADDTOCALLSTACK();
    _dir = dir;
}

Dir Artifact::get_dir() {
    ADDTOCALLSTACK();
    return _dir;
}

const Tooltip& Artifact::get_tooltip() {
    return _tooltip;
}

void Artifact::add_cliloc(udword_t id, bool dynamic) {
    Cliloc cliloc(id, dynamic);
    if (!dynamic) {
        get_cliloc_static(id);
    }
    _tooltip.add_cliloc(cliloc);
}
