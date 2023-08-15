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

#include <string>
#include <map>
#include <library/types.h>
#include <game/account.h>
#include <game/word_object_container.h>

class Account;

extern class AccountsManager : private ServerAccountsContainer {
public:

    /**
     * @brief Initializer for extern class.
     */
    void init();
    /**
     * @brief Destroyer for extern class.
     */
    void shutdown();
    bool exists(std::string name, std::string pw);
    void load_all();
    void save_all();
    bool check();
    Account *get_account(std::string name);
    bool create(std::string name, std::string pw, t_ubyte privlevel);
} torusacc;

#endif // __TORUS_GAME_ACCOUNTS_MANAGER_H