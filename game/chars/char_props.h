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

#ifndef _TORUS_GAME_CHAR_PROPS_H_
#define _TORUS_GAME_CHAR_PROPS_H_

#include <map>
#include "../../library/types.h"

enum CHAR_RACE {
    RACE_HUMAN,
    RACE_ELF,
    RACE_GARGOYLE,
    RACE_QTY
};

enum CHAR_GENDER {
    GENDER_MALE,    ///< Male character.
    GENDER_FEMALE,  ///< Female character.
    GENDER_QTY      ///< Monsters.
};

enum CHAR_PROP {
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
    PROP_LOWERREAGENTCOST,
};

class CharStats;

class CharProps {
public:
    CharProps();
    ~CharProps();
// Races & genders
private:
    CHAR_RACE _race;
    CHAR_GENDER _gender;
public:
    void set_race(CHAR_RACE race);
    CHAR_RACE get_race();
    void set_gender(CHAR_GENDER gender);
    CHAR_GENDER get_gender();
    bool can_equip(t_udword iflags);

// Props
private:
    std::map<CHAR_PROP, t_qword> _props;
public:
    void set_prop(CHAR_PROP prop, t_dword val);
    t_qword get_prop(CHAR_PROP prop);
private:
    t_word _fame;
    t_word _karma;
public:
    void set_fame(t_word val);
    t_word get_fame();
    void set_karma(t_word val);
    t_word get_karma();
private:
    t_udword _body;  //TODO CHAR_TYPES read from scripts.
    t_udword _obody;
public:
    void set_body(t_udword body);
    t_udword get_body();
    void set_obody(t_udword obody);
    t_udword get_obody(t_udword obody);
private:
    t_udword _ocolor;
public:
    void set_ocolor(t_udword ocolor);
    t_udword get_ocolor();

};

#endif //_TORUS_GAME_CHAR_PROPS_H_