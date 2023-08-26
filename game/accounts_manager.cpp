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
#include <library/system_headers.h>
#include <game/accounts_manager.h>
#include <shell.h>
#include <debug_support/debug.h>
#include <game/account.h>
#include <debug_support/callstack.h>
#include <game/world_object_container.h>
#include <game/server.h>


void AccountsManager::init()
{
    ADDTOCALLSTACK();
    create("test", "a", 8);
}

void AccountsManager::shutdown()
{
    ADDTOCALLSTACK();
}

bool AccountsManager::exists(std::string name, std::string pw)
{
    ADDTOCALLSTACK();
    return has(name.c_str());
}

void AccountsManager::load_all()
{
    ADDTOCALLSTACK();
}

void AccountsManager::save_all()
{
    ADDTOCALLSTACK();
}

bool AccountsManager::check()
{
    ADDTOCALLSTACK();
    return true;
}

Account * AccountsManager::get_account(std::string name) {
    ADDTOCALLSTACK();
    Account *acc = nullptr;
    if (!name.empty())
    {
        /*for (auto& key : _list)
        {
            if (key.first == name)
            {
                acc = key.second;
            }
        }*/
        if (has(name.c_str()))
        {
            acc = server.get_account(get(name));
        }
    }
    return acc;
}

bool AccountsManager::create(std::string name, std::string pw, t_ubyte privlevel) {
    ADDTOCALLSTACK();
    if (exists(name, pw)) {
        DEBUG_ERROR("Error creating account '" << name << "', reason: Account already exists.");
        return false;
    }
    Account* acc = new Account(name, pw, (PRIVLVL)privlevel);
    server.add_account(acc);
    add(name, acc->get_uid());
    return false;
}
