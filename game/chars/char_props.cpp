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

#include "../account.h"
#include "../../debug/callstack.h"
#include "char_props.h"
#include "char_stats.h"
#include "../item.h"
#include "../char.h"
#include "../server.h"

CharProps::CharProps(Char *whoami) {
    _char = whoami;
    _race = RACE_QTY;
    _gender = GENDER_QTY;
    _fame = 0;
    _karma = 0;
    _body = 400;    //TODO update value with any Body when they get implemented.
    _obody = 400;
    _ocolor = 0;
    _dir = N;
}

CharProps::~CharProps() {
    mark_db_delete();
}

bool CharProps::db_load(pqxx::result::const_iterator r) {
    _race = (CHAR_RACE)r[COLNAME_CHARPROPS_RACE].as<t_ubyte>();
    _gender = (CHAR_GENDER)r[COLNAME_CHARPROPS_GENDER].as<t_ubyte>();
    _fame = r[COLNAME_CHARPROPS_FAME].as<t_word>();
    _karma = r[COLNAME_CHARPROPS_KARMA].as<t_word>();
    _body = r[COLNAME_CHARPROPS_BODY].as<t_udword>();
    _obody = r[COLNAME_CHARPROPS_OBODY].as<t_udword>();
    _ocolor = r[COLNAME_CHARPROPS_OCOLOR].as<t_udword>();
    _dir = (DIR)r[COLNAME_CHARPROPS_DIR].as<t_ubyte>();
    return false;
}

bool CharProps::db_save() {
    if (!db_update)
        return false;
    std::stringstream query;
    query << "UPDATE " << TABLENAME_CHARPROPS <<
        " SET " << COLNAME_CHARPROPS_RACE << " = " << (int)_race <<
        " SET " << COLNAME_CHARPROPS_GENDER << " = " << (int)_gender <<
        " SET " << COLNAME_CHARPROPS_FAME << " = " << _fame <<
        " SET " << COLNAME_CHARPROPS_KARMA << " = " << _karma <<
        " SET " << COLNAME_CHARPROPS_BODY << " = " << _body <<
        " SET " << COLNAME_CHARPROPS_OBODY << " = " << _obody <<
        " SET " << COLNAME_CHARPROPS_OCOLOR << " = " << _ocolor <<
        " SET " << COLNAME_CHARPROPS_DIR << " = " << _dir <<
        " WHERE " << COLNAME_CHARPROPS_UID << " = " << _char->get_uid(); // TODO: pw encryption.
    return true;
}

void CharProps::mark_db_update() {
    server.update_obj(this);
}

void CharProps::mark_db_delete() {
    ADDTOCALLSTACK();
    std::stringstream query;
    query << "DELETE FROM " << TABLENAME_CHARPROPS << " WHERE " << COLNAME_CHARPROPS_UID << " = " << _char->get_uid();
    torusdb.exec(query.str());
}

void CharProps::set_race(CHAR_RACE race) {
    ADDTOCALLSTACK();
    _race = race;
    mark_db_update();
}

CHAR_RACE CharProps::get_race() {
    ADDTOCALLSTACK();
    return _race;
}

void CharProps::set_gender(CHAR_GENDER gender) {
    ADDTOCALLSTACK();
    _gender = gender;
    mark_db_update();
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
    if ((iflags & CAN_EQUIP_MALE_ONLY) && _gender != GENDER_MALE)
        return false;
    if ((iflags & CAN_EQUIP_FEMALE_ONLY) && _gender != GENDER_FEMALE)
        return false;
    if (_race == RACE_HUMAN && !(iflags & CAN_EQUIP_HUMAN))
        return false;
    if (_race == RACE_ELF && !(iflags & CAN_EQUIP_ELF))
        return false;
    if (_race == RACE_GARGOYLE && !(iflags & CAN_EQUIP_GARGOYLE))
        return false;
    return true;
}

void CharProps::set_prop(CHAR_PROP prop, t_dword val) {
    ADDTOCALLSTACK();
    _props[prop] = val;
    mark_db_update();
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
    mark_db_update();
}

t_word CharProps::get_fame() {
    ADDTOCALLSTACK();
    return _fame;
}

void CharProps::set_karma(t_word val) {
    ADDTOCALLSTACK();
    _karma = val;
    mark_db_update();
}

t_word CharProps::get_karma() {
    ADDTOCALLSTACK();
    return _karma;
    mark_db_update();
}

void CharProps::set_body(t_udword body) {
    ADDTOCALLSTACK();
    _body = body;
    mark_db_update();
}

t_udword CharProps::get_body() {
    ADDTOCALLSTACK();
    return _body;
}

void CharProps::set_obody(t_udword obody) {
    ADDTOCALLSTACK();
    _obody = obody;
    mark_db_update();
}

t_udword CharProps::get_obody() {
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

void CharProps::set_dir(DIR dir) {
    ADDTOCALLSTACK();
    _dir = dir;
    mark_db_update();
}

DIR CharProps::get_dir() {
    ADDTOCALLSTACK();
    return _dir;
}

Char * CharProps::get_char() {
    return _char;
}

t_uword CharProps::get_status_flag(Char *viewer) {
    ADDTOCALLSTACK();
    t_uword flags = SF_NORMAL;
    t_udword cflags = _char->get_flags();
    // if (flags freezed / stoned)
    //  _status_flags |= SF_FREEZED;
    if (get_gender() == GENDER_FEMALE)
        flags |= SF_FEMALE;
    if (get_race() == RACE_GARGOYLE /* && cflags & FLAG_FLYING && (viewer->get_client()->get_version() >= CLIENT_SA || viewer->get_client()->is_enhanced())*/) { // No need to check this for viewers who cannot see gargoyles and their flying mode.
        flags |= SF_FLYING;
    }
    if (cflags & CFLAG_POISONED) {  // FLYING and POISONED share the same flag.
        flags |= SF_POISONED;
    }
    if (cflags & CFLAG_INVUL) {
        flags |= SF_INVUL;
    }
    if (cflags & CFLAG_WAR) {
        flags |= SF_WARMODE;
    }
    if (_char->get_account() && _char->get_account()->get_privlevel() > PRIV_PLAYER) {
        flags |= SF_IGNORECHARS;
    }
    if (cflags & (CFLAG_INVIS | CFLAG_HIDDEN)) {
        flags |= SF_INVIS;
    }
    return flags;
}
