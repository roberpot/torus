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

    mark_db_update();
}

bool Account::db_load(pqxx::result::const_iterator r) {
    ADDTOCALLSTACK();
    try {
        _id = r[COLNAME_ACCOUNTS_ID].as<t_uqword>();
        _name = r[COLNAME_ACCOUNTS_NAME].as<std::string>();
        _privlevel = static_cast<PRIVLVL>(r[COLNAME_ACCOUNTS_PRIVLEVEL].as<int>());
        _password = r[COLNAME_ACCOUNTS_PASSWORD].as<std::string>();
        _flags = 0;
        _expansion = 0;
        _lastip = "";
    }
    catch (const pqxx::sql_error &e) {
        std::cout << "Exec failed with error: " << e.what() << "\n for query :" << e.query() << std::endl;
        return false;
    }
    return true;
}

bool Account::db_save() {
    ADDTOCALLSTACK();
    std::stringstream query;
    pqxx::result r;
    query << "UPDATE " << TABLENAME_ACCOUNTS <<
        " SET " << COLNAME_ACCOUNTS_NAME << " = " << _name <<
        " SET " << COLNAME_ACCOUNTS_PRIVLEVEL << " = " << _privlevel <<
        " WHERE " COLNAME_ACCOUNTS_ID << " = " << _id; // TODO: pw encryption.
    bool succeed = torusdb.exec(query.str(), r);
    return succeed;
}

void Account::mark_db_update() {
    ADDTOCALLSTACK();
    torusacc.update_obj(this);
}

void Account::mark_db_delete() {
    ADDTOCALLSTACK();
    std::stringstream query;
    query << "DELETE FROM " << TABLENAME_ACCOUNTS << " WHERE " << COLNAME_ACCOUNTS_ID << " = " << get_id();
    torusdb.exec(query.str());
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
            delete chr;
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
    mark_db_update();
}

void Account::remove() {
    ADDTOCALLSTACK();
    for (t_byte i = 0; i < _charlist.size(); i++) {
        Char *chr = _charlist[i];
        if (chr) {
            delete chr;
            chr = 0;
        }
    }
    delete this;
}
