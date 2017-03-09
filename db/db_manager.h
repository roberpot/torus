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

#ifndef __TORUS_DB_DBMANAGER_H
#define __TORUS_DB_DBMANAGER_H

#include <map>
#include "../library/system_headers.h"
#include "../library/types.h"
#include "torus_pg_transaction.h"
#include "torus_db_table.h"

extern class DBManager {
public:
    DBManager();
    ~DBManager();
    bool init();
private:
    pqxx::connection *_conn;
    std::string _db_host;
    std::string _db_name;
    std::string _db_user;
    std::string _db_password;
    std::string _table_accounts;
    void read_cfg();
public:
    bool connect();
private:
    std::vector<TDBTable *> _tables;
    void add_table(TDBTable * table);
    bool run_tests();
public:
    bool exec(std::string query, pqxx::result &r);
    bool exec(std::string query);
private:
    std::map<torus_thread_id, PGTransaction*> _works;
public:
    template<typename T>
    friend DBManager & operator<<(DBManager & db, T & d);
    PGTransaction *create_transaction();
    PGTransaction *get_transaction();
    bool has_transaction();
    void delete_transaction();
} torusdb;

template<typename T>
DBManager & operator <<(DBManager & db, T & d) {
    if (!db.has_statement()) {
        db.create_statement();
    }
    db._works[get_current_thread_id()] << d;
    return db;
}
#endif // __TORUS_DB_DBMANAGER_H