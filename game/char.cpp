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

#include <library/system_headers.h>
#include <debug_support/callstack.h>

#include <game/account.h>
#include <game/char.h>
#include <game/client.h>
#include <game/item.h>
#include <game/server.h>
#include <game/uid_manager.h>

Char::Char() : Artifact(UidMask::UID_CHAR) {
    ADDTOCALLSTACK();
    _account = nullptr;
    _client = nullptr;
    _body = BodyType::BODY_HUMAN_MALE;
    server.add_char(this);
}

Char::Char(const Char& other)
{
    memcpy(_stats, other._stats, int(StatType::STATS_QTY));
    _account = other._account;
    _client = other._client;
    _body = other._body;

    server.add_char(this);
}

Char::Char(udword_t uid) : Artifact(uid) {
    ADDTOCALLSTACK();
    _account = nullptr;
    _client = nullptr;
    _body = BodyType::BODY_HUMAN_MALE;
    //server.add_char(this);
}

Char::~Char(){
    ADDTOCALLSTACK();
}

CharStats &Char::get_stat(StatType key){
    ADDTOCALLSTACK();
    return _stats[int(key)];
}

bool Char::can_move(){
    ADDTOCALLSTACK();
    if (has_flag(CFLAG_GM)) {
        return true;
    }
    if (!has_flag(CFLAG_CAN_MOVE)) {
        return false;
    }
    return true;
}

void Char::remove() {
    server.del_char(this);
}

bool Char::tick() {
    ADDTOCALLSTACK();
    return true;
}

bool Char::can_see(Char * target) {
    ADDTOCALLSTACK();
    if (get_distance(target) > DISTANCE_SEE_CHARS) {
        return false;
    }
    if (target->get_flags() & CFLAG_INVIS && target->get_account()->get_privlevel() > get_account()->get_privlevel()) {
        return false;
    }
    if (target->get_flags() & CFLAG_HIDDEN && target->get_account()->get_privlevel() >= get_account()->get_privlevel()) {
        // same privlevel or higher than me? return false. TODO: detecting hidden pasive checks?
        return false;
    }
    return true;
}

bool Char::can_see(Item * target) {
    ADDTOCALLSTACK();
    if (get_distance(target) > DISTANCE_SEE_ITEMS)
        return false;
    // TODO: attr_invis check
    return true;
}

Account * Char::get_account() {
    ADDTOCALLSTACK();
    return _account;
}

void Char::set_account(Account * account) {
    ADDTOCALLSTACK();
    _account = account;
}

void Char::attach_client(Client* client)
{
    _client = client;
}

void Char::detatch_client()
{
    _client = nullptr;
}

Client* Char::get_client()
{
    return _client;
}

uword_t Char::get_status_flag(Char *viewer) {
    ADDTOCALLSTACK();
    uword_t flags = SF_NORMAL;
    udword_t cflags = get_flags();
    // if (flags freezed / stoned)
    //  _status_flags |= SF_FREEZED;
    if (get_gender() == CharGender::GENDER_FEMALE)
        flags |= SF_FEMALE;
    if (get_race() == CharRace::RACE_GARGOYLE /* && cflags & FLAG_FLYING && (viewer->get_client()->get_version() >= CLIENT_SA || viewer->get_client()->is_enhanced())*/) { // No need to check this for viewers who cannot see gargoyles and their flying mode.
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
    if (get_account() && get_account()->get_privlevel() > PRIV_PLAYER) {
        flags |= SF_IGNORECHARS;
    }
    if (cflags & (CFLAG_INVIS | CFLAG_HIDDEN)) {
        flags |= SF_INVIS;
    }
    if (viewer->get_account()->get_privlevel() > get_account()->get_privlevel()) {

    }
    return flags;
}

bool Char::can_equip(udword_t iflags) {
    ADDTOCALLSTACK();
    if (iflags == CAN_EQUIP_ALL)
        return true;
    if (iflags & CAN_EQUIP_NONE)
        return false;
    if ((iflags & CAN_EQUIP_MALE_ONLY) && get_gender() != CharGender::GENDER_MALE)
        return false;
    if ((iflags & CAN_EQUIP_FEMALE_ONLY) && get_gender() != CharGender::GENDER_FEMALE)
        return false;
    if (get_race() == CharRace::RACE_HUMAN && !(iflags & CAN_EQUIP_HUMAN))
        return false;
    if (get_race() == CharRace::RACE_ELF && !(iflags & CAN_EQUIP_ELF))
        return false;
    if (get_race() == CharRace::RACE_GARGOYLE && !(iflags & CAN_EQUIP_GARGOYLE))
        return false;
    return true;
}

BodyType Char::get_body()
{
    return _body;
}

void Char::set_body(BodyType body)
{
    ADDTOCALLSTACK();
    _body = body;
}
