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

#include "char.h"
#include "../debug/callstack.h"

Char::Char() : Artifact(UID_CHAR), CharProps(this){
    ADDTOCALLSTACK();
    _account = NULL;
}

Char::Char(t_udword uid) : Artifact(uid), CharProps(this) {
    ADDTOCALLSTACK();
    _account = NULL;
}

Char::~Char(){
    ADDTOCALLSTACK();
}

CharStats &Char::get_stat(STAT_TYPE key){
    ADDTOCALLSTACK();
    return _stats[key];
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

Account * Char::get_account() {
    ADDTOCALLSTACK();
    return _account;
}

void Char::set_account(Account * account) {
    ADDTOCALLSTACK();
    _account = account;
}
