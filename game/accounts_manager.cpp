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
#include <game/accounts_manager.h>
#include <shell.h>
#include <debug_support/debug.h>
#include <game/account.h>
#include <debug_support/callstack.h>

AccountsManager torusacc;

AccountsManager::AccountsManager()
{
    create("nefthon", "a", 8);
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
    return _list.find(name) != _list.end();
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

Account * AccountsManager::get_account(std::string name)
{
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
        if (_list.count(name.c_str()) != 0)
        {
            acc = _list[name];
        }      
    }
    return acc;
}

bool AccountsManager::create(std::string name, std::string pw, t_ubyte privlevel)
{
    ADDTOCALLSTACK();
    if (exists(name, pw)) {
        DEBUG_ERROR("Error creating account '" << name << "', reason: Account already exists.");
        return false;
    }
    _list[name] = new Account(name, pw, (PRIVLVL)privlevel);
    return false;
}
