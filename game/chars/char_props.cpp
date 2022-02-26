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

#include <game/account.h>
#include <debug_support/callstack.h>
#include <game/chars/char_props.h>
#include <game/char.h>
#include <game/server.h>

CharProps::CharProps() {
    ADDTOCALLSTACK();
    _race = CharRace::RACE_QTY;
    _gender = CharGender::GENDER_QTY;
    _fame = 0;
    _karma = 0;
    _body = 400;    //TODO update value with any Body when they get implemented.
    _obody = 400;
    _ocolor = 0;
    _dir = Dir::N;
}

CharProps::~CharProps() {
    ADDTOCALLSTACK();
}

void CharProps::set_race(CharRace race) {
    ADDTOCALLSTACK();
    _race = race;
}

CharRace CharProps::get_race() {
    ADDTOCALLSTACK();
    return _race;
}

void CharProps::set_gender(CharGender gender) {
    ADDTOCALLSTACK();
    _gender = gender;
}

CharGender CharProps::get_gender() {
    ADDTOCALLSTACK();
    return _gender;
}

void CharProps::set_prop(CharProp prop, dword_t val) {
    ADDTOCALLSTACK();
    _props[prop] = val;
}

qword_t CharProps::get_prop(CharProp prop) {
    ADDTOCALLSTACK();
    qword_t ret = 0;
    if (_props[prop])
        ret = _props[prop];
    return ret;
}

void CharProps::set_fame(word_t val) {
    ADDTOCALLSTACK();
    _fame = val;
}

word_t CharProps::get_fame() {
    ADDTOCALLSTACK();
    return _fame;
}

void CharProps::set_karma(word_t val) {
    ADDTOCALLSTACK();
    _karma = val;
}

word_t CharProps::get_karma() {
    ADDTOCALLSTACK();
    return _karma;
}

void CharProps::set_body(udword_t body) {
    ADDTOCALLSTACK();
    _body = body;
}

udword_t CharProps::get_body() {
    ADDTOCALLSTACK();
    return _body;
}

void CharProps::set_obody(udword_t obody) {
    ADDTOCALLSTACK();
    _obody = obody;
}

udword_t CharProps::get_obody() {
    ADDTOCALLSTACK();
    return _obody;
}

void CharProps::set_ocolor(udword_t ocolor) {
    ADDTOCALLSTACK();
    _ocolor = ocolor;
}

udword_t CharProps::get_ocolor() {
    ADDTOCALLSTACK();
    return _ocolor;
}

void CharProps::set_dir(Dir dir) {
    ADDTOCALLSTACK();
    _dir = dir;
}

Dir CharProps::get_dir() {
    ADDTOCALLSTACK();
    return _dir;
}
