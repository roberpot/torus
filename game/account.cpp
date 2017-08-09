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

#include "account.h"
#include "accounts_manager.h"
#include "char.h"
#include "../network/socket.h"
#include "../debug/callstack.h"
#include <iostream>

Account::Account() {
    _socket = 0;
}

Account::~Account(){
    ADDTOCALLSTACK();
    if (_socket)
        delete _socket;
}

t_uqword Account::get_id() {
    ADDTOCALLSTACK();
    return _id;
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
    for (t_byte i = 0; i < _charlist.size(); i++) {    // run the whole character list to find an empty slot
        Char *existingchar = _charlist[i];
        if (existingchar && existingchar == chr)
            continue;
        _charlist.push_back(chr);
        return true;
    }
    return false;
}

bool Account::delete_char(Char *chr){
    ADDTOCALLSTACK();
    for (t_byte i = 0; i < _charlist.size(); i++) {    // run the whole character list to find this character.
        Char *existingchar = _charlist[i];
        if (existingchar && existingchar == chr) {
            _charlist.erase(_charlist.begin() + i);        // delete de Char* directly.
            chr->remove();
            return true;
        }
    }
    return false;
}

void Account::connect(Socket * socket){
    ADDTOCALLSTACK();
    _socket = socket;
    _lastip = _socket->get_ip();
}

PRIVLVL Account::get_privlevel() {
    ADDTOCALLSTACK();
    return _privlevel;
}

void Account::set_privlevel(PRIVLVL lvl) {
    ADDTOCALLSTACK();
    _privlevel = lvl;
}

void Account::remove() {
    ADDTOCALLSTACK();
    for (t_byte i = 0; i < _charlist.size(); i++) {
        Char *chr = _charlist[i];
        if (chr) {
            chr->remove();
            chr = 0;
        }
    }
    delete this;
}
