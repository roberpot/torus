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

#include "../../debug/callstack.h"
#include "char_props.h"
#include "char_stats.h"
#include "../item.h"

CharProps::CharProps() {
    _race = RACE_QTY;
    _gender = GENDER_QTY;
}

CharProps::~CharProps() {
}

void CharProps::set_race(CHAR_RACE race) {
    ADDTOCALLSTACK();
    _race = race;
}

CHAR_RACE CharProps::get_race() {
    ADDTOCALLSTACK();
    return _race;
}

void CharProps::set_gender(CHAR_GENDER gender) {
    ADDTOCALLSTACK();
    _gender = gender;
}

CHAR_GENDER CharProps::get_gender() {
    ADDTOCALLSTACK();
    return _gender;
}

bool CharProps::can_equip(t_udword iflags) {
    ADDTOCALLSTACK();
    if (iflags == CAN_EQUIP_ALL)
        return true;
    if (iflags & CAN_EQUIP_NONE)
        return false;
    if (iflags & CAN_EQUIP_MALE_ONLY && _gender != GENDER_MALE)
        return false;
    if (iflags & CAN_EQUIP_FEMALE_ONLY && _gender != GENDER_FEMALE)
        return false;
    if (_race == RACE_HUMAN && !iflags & CAN_EQUIP_HUMAN)
        return false;
    if (_race == RACE_ELF && !iflags & CAN_EQUIP_ELF)
        return false;
    if (_race == RACE_GARGOYLE && !iflags & CAN_EQUIP_GARGOYLE)
        return false;
    return true;
}

void CharProps::set_prop(CHAR_PROP prop, t_dword val) {
    ADDTOCALLSTACK();
    _props[prop] = val;
}

t_qword CharProps::get_prop(CHAR_PROP prop) {
    ADDTOCALLSTACK();
    t_qword ret = 0;
    if (_props[prop])
        ret = _props[prop];
    return ret;
}

void CharProps::set_fame(t_word val) {
    ADDTOCALLSTACK();
    _fame = val;
}

t_word CharProps::get_fame() {
    ADDTOCALLSTACK();
    return _fame;
}

void CharProps::set_karma(t_word val) {
    ADDTOCALLSTACK();
    _karma = val;
}

t_word CharProps::get_karma() {
    ADDTOCALLSTACK();
    return _karma;
}

void CharProps::set_body(t_udword body) {
    ADDTOCALLSTACK();
    _body = body;
}

t_udword CharProps::get_body() {
    ADDTOCALLSTACK();
    return _body;
}

void CharProps::set_obody(t_udword obody) {
    ADDTOCALLSTACK();
    _obody = obody;
}

t_udword CharProps::get_obody(t_udword obody) {
    ADDTOCALLSTACK();
    return _obody;
}

void CharProps::set_ocolor(t_udword ocolor) {
    ADDTOCALLSTACK();
    _ocolor = ocolor;
}

t_udword CharProps::get_ocolor() {
    ADDTOCALLSTACK();
    return _ocolor;
}
