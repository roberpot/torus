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

#include "../network/socket.h"
#include "../debug/callstack.h"
#include "account.h"
#include "char.h"

Account::Account(const t_byte *name, const t_byte *pw, t_udword flags , t_uword exp ){
    ADDTOCALLSTACK();
    _name = name;
    _password = pw;
    _flags = flags;
    _expansion = exp;
    _lastip = "";
    _socket = 0;
}

Account::~Account(){
    ADDTOCALLSTACK();
    for (t_byte i = 0; i < 7; i++) {
        if (_charlist[i]) {
            delete _charlist[i];
            _charlist[i] = 0;
        }
    }
    if (_socket)
        delete _socket;
}

t_byte Account::get_char_count(){
    ADDTOCALLSTACK();
    t_byte count = 0;
    for (t_byte i = 0; i < 7; i++) {    // run the whole character's list.
        if (_charlist[i])   // increase count if a character is found.
            count++;
    }
    return count;
}

t_byte Account::get_max_chars(){
    ADDTOCALLSTACK();
    if (_expansion & EXP_SA)    // Accounts with SA Expansion can have 7 characters
        return 7;
    return 5;   // default = 5 chars
    // TODO: proper expansion checks to retrieve the maximum amount of characters for this account.
}

bool Account::can_add_char(){
    ADDTOCALLSTACK();
    if (get_char_count() >= get_max_chars())    // check if still has free character slots.
        return false;
    return true;
}

bool Account::add_char(Char *chr){
    ADDTOCALLSTACK();
    for (t_byte i = 0; i < 7; i++) {    // run the whole character list to find an empty slot
        if (_charlist[i])
            continue;
        _charlist[i] = chr;
        return true;
    }
    return false;
}

bool Account::delete_char(Char *chr){
    ADDTOCALLSTACK();
    for (t_byte i = 0; i < 7; i++) {    // run the whole character list to find this character.
        if (_charlist[i] == chr) {
            delete _charlist[i];        // delete de Char* directly.
            _charlist[i] = 0;           // clear this slot.
            fix_charlist();             // reorder the whole list to avoid empty slots between characters.
            return true;
        }
    }
    return false;
}

void Account::fix_charlist(){
    ADDTOCALLSTACK();
    for (t_byte i = 0; i < 7; i++) {
        if (!_charlist[i] && _charlist[i+1]) {
            _charlist[i] = _charlist[i+1];
            _charlist[i+1] = 0;
        }
    }
}

void Account::connect(Socket * socket){
    ADDTOCALLSTACK();
    _socket = socket;
    _lastip = _socket->get_ip();
}