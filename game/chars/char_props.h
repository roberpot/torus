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

#ifndef __TORUS_GAME_CHAR_PROPS_H_
#define __TORUS_GAME_CHAR_PROPS_H_

#include <map>

#include <library/types.h>
#include <game/enums.h>

enum class CharProp {
    PROP_BLOODCOLOR,
    PROP_CURFOLLOWER,
    PROP_MAXFOLLOWER,
    PROP_FASTERCASTING,
    PROP_FASTERCASTRECOVERY,
    PROP_FIGHTRANGE,
    PROP_INCREASEDAM,
    PROP_INCREASEDEFCHANCE,
    PROP_INCREASEDEFCHANCEMAX,
    PROP_INCREASEHITCHANCE,
    PROP_INCREASESPELLDAM,
    PROP_INCREASESWINGSPEED,
    PROP_LOWERMANACOST,
    PROP_LOWERREAGENTCOST
};

#define SF_NORMAL       0x00
#define SF_FREEZED      0x01
#define SF_FEMALE       0x02
#define SF_POISONED     0x04
#define SF_FLYING       0x04
#define SF_INVUL        0x08
#define SF_IGNORECHARS  0x10
#define SF_UNK1         0x20
#define SF_WARMODE      0x40
#define SF_INVIS        0x80

class Char;
class CharStats;

class CharProps {
public:
    CharProps();
    ~CharProps();

// Races & genders
private:
    CharRace _race;
    CharGender _gender;
public:
    void set_race(CharRace race);
    CharRace get_race();
    void set_gender(CharGender gender);
    CharGender get_gender();

// Props
private:
    std::map<CharProp, qword_t> _props;
public:
    void set_prop(CharProp prop, dword_t val);
    qword_t get_prop(CharProp prop);

// Misc
private:
    word_t _fame;
    word_t _karma;
public:
    void set_fame(word_t val);
    word_t get_fame();
    void set_karma(word_t val);
    word_t get_karma();
private:
    udword_t _body;  //TODO CHAR_TYPES read from scripts.
    udword_t _obody;
public:
    void set_body(udword_t body);
    udword_t get_body();
    void set_obody(udword_t obody);
    udword_t get_obody();
private:
    udword_t _ocolor;
public:
    void set_ocolor(udword_t ocolor);
    udword_t get_ocolor();

};

#endif //__TORUS_GAME_CHAR_PROPS_H_