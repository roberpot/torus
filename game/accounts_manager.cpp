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

#include <iostream>
#include "accounts_manager.h"
#include "../db/db_manager.h"
#include "../shell.h"
#include "../debug/debug.h"
#include "account.h"
#include "../debug/callstack.h"

AccountsManager torusacc;

AccountsManager::AccountsManager() {
}

AccountsManager::~AccountsManager() {
}

void AccountsManager::init() {
    ADDTOCALLSTACK();
    /*std::stringstream query; 
    pqxx::result r;
    query << "SELECT " << COLNAME_ACCOUNTS_NAME << " FROM accounts WHERE privlevel >= " << PRIV_ADMIN;
    torusdb.exec(query.str(), r);
    if (!r.size()) {
        TORUSSHELLECHO("There are no accounts with Admin privileges, creating default account.");
        std::stringstream acc;
        do {
            acc.str(std::string());
            acc.clear();
            acc << "admin" << std::rand() % 1000;
        } while (!create(acc.str(), "adminpw", 6));
    }
    else {
        load_all();
    }*/
}

bool AccountsManager::exists(std::string name, std::string pw) {
    ADDTOCALLSTACK();
    std::stringstream query;
    pqxx::result r;
    query << "SELECT " << COLNAME_ACCOUNTS_ID << " FROM accounts WHERE " << COLNAME_ACCOUNTS_NAME << " = '" << name.c_str() << "'";
    if (pw.size()) {
        query << " AND password = '" << pw.c_str() << "'";
    }
    torusdb.exec(query.str(), r);
    if (r.size()) {
        return true;
    }
    return false;
}

void AccountsManager::load_all() {
    ADDTOCALLSTACK();
    pqxx::result acclist;
    std::stringstream query;
    query << "SELECT * from " << TABLENAME_ACCOUNTS << " LIMIT 1";
    torusdb.exec(query.str(), acclist);
    
    for (pqxx::result::const_iterator row = acclist.begin(); row != acclist.end(); ++row) {
        Account *acc = new Account();
        acc->db_load(row);
        _list[acc->get_id()] = acc;
        ++row;
    }
}

void AccountsManager::save_all() {
    ADDTOCALLSTACK();
}

bool AccountsManager::check() {
    ADDTOCALLSTACK();
    std::stringstream query;
    query << "SELECT id, name, password, privlevel from " << TABLENAME_ACCOUNTS << " LIMIT 1";
    return torusdb.exec(query.str());
}

Account * AccountsManager::get_account(std::string name) {
    ADDTOCALLSTACK();
    return get_account(get_account_id(name));
}

Account * AccountsManager::get_account(t_uqword id) {
    ADDTOCALLSTACK();
    if (id > 0) {
        return _list[id];
    }
    return NULL;
}

t_uqword AccountsManager::get_account_id(std::string name) {
    ADDTOCALLSTACK();
    PGTransaction *trstn = torusdb.create_transaction();
    std::stringstream query;
    pqxx::result r;
    query << "SELECT " << COLNAME_ACCOUNTS_ID << " FROM " << TABLENAME_ACCOUNTS << " WHERE " << COLNAME_ACCOUNTS_NAME << " = " << trstn->quote(name);
    torusdb.exec(query.str(), r);
    if (r.size()) {
        return r[0][0].as<t_uqword>();
    }
    return 0;
}

bool AccountsManager::create(std::string name, std::string pw, t_ubyte privlevel) {
    ADDTOCALLSTACK();
    if (exists(name, pw)) {
        DEBUG_ERROR("Error creating account '" << name << "', reason: Account already exists.");
        return false;
    }
    PGTransaction *trstn = torusdb.create_transaction();
    std::stringstream query;
    pqxx::result r;
    query << "INSERT INTO " << TABLENAME_ACCOUNTS << "(name, password, privlevel) VALUES (" << trstn->quote(name) << ", " << trstn->quote(pw) << ", " << (int)privlevel << ")"; // TODO: pw encryption.
    bool succeed = torusdb.exec(query.str(), r);
    if (succeed) {
        DEBUG_INFO("Created account '" << name << "' with privlevel = " << (int)privlevel);
        Account *acc = new Account();
        acc->db_load(r.begin());
        _list[acc->get_id()] = acc;
        return true;
    }
    DEBUG_ERROR("Failed to create account '" << name << "'.");
    return false;
}
