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
#include "db_manager.h"
#include "../core/config.h"
#include "../core/errors.h"
#include <iostream>
#include "../debug/debug.h"
#include "pqxx/transaction"
#include "../debug/callstack.h"
#include "../game/accounts_manager.h"

DBManager torusdb;

DBManager::DBManager() {
}

DBManager::~DBManager() {
    ADDTOCALLSTACK();
    delete _conn;
    // TODO: check any pending data outgoing and send it at once.
}

bool DBManager::init() {
    ADDTOCALLSTACK();
    read_cfg();
    if (!connect()) {
        return false;
    }
    //add_table(torusacc);
    return run_tests();
}

void DBManager::read_cfg() {
    ADDTOCALLSTACK();
    _db_host = toruscfg._dbsettings._db_host;
    _db_name = toruscfg._dbsettings._db_name;
    _db_user = toruscfg._dbsettings._db_user;
    _db_password = toruscfg._dbsettings._db_password;
    _table_accounts = toruscfg._dbsettings._table_accounts;
}

bool DBManager::connect() {
    ADDTOCALLSTACK();
    std::stringstream ss;
    ss  << "user = " << _db_user << " password = " << _db_password << " dbname = " <<_db_name;
    try{
        _conn = new pqxx::connection(ss.str());
    }
    catch (const pqxx::pqxx_exception &e) {
        std::cout << "Connection failed with error: " << e.base().what() << std::endl;
        //THROW_ERROR(PostgresError, "Connection failed with error: " << e.base().what());
        return false;
    }
    DEBUG_INFO("Connected sucessfully to DataBase '" << _db_name << "'.");
    return true;
}

void DBManager::add_table(TDBTable * table) {
    ADDTOCALLSTACK();
    if (_tables.size()) {
        for (size_t i = 0; i < _tables.size(); ++i) {
            TDBTable * tb = _tables[i];
            if (tb && tb == table) {
                return; // already have this table, avoiding it.
            }
        }
    }
    _tables.push_back(table);
}

bool DBManager::run_tests() {
    ADDTOCALLSTACK();
    t_uword fails = 0;
    if (_tables.size()) {
        for (size_t i = 0; i < _tables.size(); ++i) {
            TDBTable * table = _tables[i];
            if (table) {
                if (!table->check()) {
                    fails++;
                }
            }
        }
    }
    if (fails > 0) {
        DEBUG_ERROR("A total of " << fails << " tables failed to load out of " << _tables.size() << "." << std::endl);
        //THROW_ERROR(PostgresError, "Connection failed with error: " << e.base().what());
        return false;
    }
    return true;
}

bool DBManager::exec(std::string query, pqxx::result & r) {
    ADDTOCALLSTACK();
    try {
        PGTransaction *trsn = get_transaction();
        if (!trsn) {
            trsn = create_transaction();
        }
        r = trsn->exec(query.c_str());
        trsn->commit();
        delete_transaction();
    }
    catch (const pqxx::sql_error &e) {
        std::cout << "Exec failed with error: " << e.what() << "\n for query :" << e.query() << std::endl;
        //THROW_ERROR(PostgresError, "Connection failed with error: " << e.base().what());
        return false;
    }
    return true;
}

bool DBManager::exec(std::string query) {
    ADDTOCALLSTACK();
    try {
        PGTransaction *trsn = get_transaction();
        if (!trsn) {
            trsn = create_transaction();
        }
        trsn->exec(query.c_str());
        trsn->commit();
        delete_transaction();
    }
    catch (const pqxx::sql_error &e) {
        std::cout << "Exec for query '" << e.query() << "' failed with error : " << e.what() <<  std::endl;
        //THROW_ERROR(PostgresError, "Connection failed with error: " << e.base().what());
        return false;
    }
    return true;
}

PGTransaction * DBManager::create_transaction() {
    ADDTOCALLSTACK();
    PGTransaction *transaction = new PGTransaction(_conn);
    _works[get_current_thread_id()] = transaction;
    return transaction;
}

PGTransaction * DBManager::get_transaction() {
    ADDTOCALLSTACK();
    return _works[get_current_thread_id()];
}

bool DBManager::has_transaction() {
    ADDTOCALLSTACK();
    if (get_transaction()) {
        return true;
    }
    return false;
}

void DBManager::delete_transaction() {
    ADDTOCALLSTACK();
    PGTransaction *pgt = get_transaction();
    delete pgt;
    _works[get_current_thread_id()] = NULL;
}
