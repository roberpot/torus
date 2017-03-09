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

#ifndef __TORUS_GAME_ACCOUNTS_MANAGER_H
#define __TORUS_GAME_ACCOUNTS_MANAGER_H

#include "../db/torus_db_table.h"
#include <string>
#include <map>
#include "../library/types.h"
#include "account.h"

#define TABLENAME_ACCOUNTS "accounts"
#define COLNAME_ACCOUNTS_ID "id"
#define COLNAME_ACCOUNTS_NAME "name"
#define COLNAME_ACCOUNTS_PASSWORD "password"
#define COLNAME_ACCOUNTS_PRIVLEVEL "privlevel"

class Account;

extern class AccountsManager : public TDBTable {
public:
    AccountsManager();
    ~AccountsManager();
private:
    std::map<t_uqword, Account*> _list;
public:
    void init();
    bool exists(std::string name, std::string pw);
    void load_all();
    void save_all();
    bool check();
    Account *get_account(std::string name);
    Account *get_account(t_uqword id);
    t_uqword get_account_id(std::string name);
    bool create(std::string name, std::string pw, t_ubyte privlevel);
} torusacc;

#endif // __TORUS_GAME_ACCOUNTS_MANAGER_H