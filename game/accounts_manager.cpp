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
#include "../shell.h"
#include "../debug/debug.h"
#include "account.h"
#include "../debug/callstack.h"

AccountsManager torusacc;

AccountsManager::AccountsManager()
{
}

AccountsManager::~AccountsManager()
{
}

void AccountsManager::init()
{
    ADDTOCALLSTACK();
}

bool AccountsManager::exists(std::string name, std::string pw)
{
    ADDTOCALLSTACK();
    return false;
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

Account * AccountsManager::get_account(size_t id)
{
    ADDTOCALLSTACK();
    if (id > 0) {
        return _list[id];
    }
    return NULL;
}

bool AccountsManager::create(std::string name, std::string pw, t_ubyte privlevel)
{
    ADDTOCALLSTACK();
    if (exists(name, pw)) {
        DEBUG_ERROR("Error creating account '" << name << "', reason: Account already exists.");
        return false;
    }
    _list.push_back(new Account(name, pw, (PRIVLVL)privlevel));
    return false;
}
