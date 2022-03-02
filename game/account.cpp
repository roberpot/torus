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
#include <game/account.h>
#include <game/accounts_manager.h>
#include <game/char.h>
#include <game/client.h>
#include <game/server.h>
#include <game/uid.h>
#include <network/socket.h>
#include <debug_support/callstack.h>

Account::Account() {
    ADDTOCALLSTACK();
    _socket = nullptr;
    _client = nullptr;
}

Account::~Account(){
    ADDTOCALLSTACK();
    if (_socket)
        _socket->set_read_closed();
}

Account::Account(std::string accname, std::string accpw, PRIVLVL accpriv)
{
    ADDTOCALLSTACK();
    _name = accname;
    _password = accpw;
    _privlevel = accpriv;
    _socket = nullptr;
    _client = nullptr;

    //FIXME: Remove this when a proper save system get's implemented.
    Char *character = new Char();
    character->set_name("XuN");
    for (int i = int(StatType::STR); i < int(StatType::SECONDARY_STAT_QTY); ++i)
    {
        character->get_stat(StatType(i)).set_base(100);
    }
    //character->set_pos(633, 858, 0, 0);
    add_char(character);
    _character = character;//FIXME DELETE

}

t_byte Account::get_char_count() {
    ADDTOCALLSTACK();
    return t_byte(_charlist.size());
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
    if (!_charlist.empty())
    {
        if (std::find(_charlist.begin(), _charlist.end(), chr->get_uid()) == _charlist.end())
        {
            //Already inserted.
            return false;
        }
    }    
    _charlist.push_back(chr->get_uid());
    return false;
}

Char* Account::get_char(const udword_t& slot)
{
    return _character;//FIXME
    Char *character = nullptr;
    if (_charlist.size() > slot)
    {
        character = server.get_char(_charlist[slot]);
    }
    return character;
}

bool Account::remove_char(Char *chr){
    ADDTOCALLSTACK();
    std::vector<Uid>::iterator it = std::find(_charlist.begin(), _charlist.end(), chr->get_uid());
    if (it == _charlist.end())
    {
        //Not found
        return false;
    }
    _charlist.erase(it);    
    return true;
}

bool Account::connect(Socket * newsocket){
    ADDTOCALLSTACK();
    if (_socket != nullptr) //Already connected? Check if it's a swap between loginserver and gameserver
    {
        bool is_connected_to_game = _socket->get_server_type() == ConnectionType::CONNECTIONTYPE_GAMESERVER;
        bool is_connecting_to_game = newsocket->get_server_type() == ConnectionType::CONNECTIONTYPE_GAMESERVER;

        if (is_connected_to_game) //Already playing
        {
            // Should block connection if already playing ...?
            // ... or should disconnect it? 
            //      May be a player whom client crashed and it's connecting again, so a disconnect of the previous socket is prefered.
            //      May be oether casualities, which we should mostly not contemplate.
            _socket->set_read_write_closed();
        }
        else // Already on loginserver
        {
            if (is_connecting_to_game)
            {
                if (_socket->get_seed() == newsocket->get_seed())
                {
                    _socket->set_read_write_closed();
                }
                else
                {
                    //Seed missmatch, must log-in first and use the seed providen by the loginserver.
                    /*DEBUG_NOTICE("Attempting to login to account " << _name << " with seed '" << newsocket->get_seed() << "', expected seed = '" << _socket->get_seed() << "'."); // FIXME: Sometimes seed is not sent.
                    newsocket->set_read_write_closed();
                    return false;*/
                }
            }
            else //Another connection to this account?
            {
                // Do nothing at the moment, so last connection prevails.
            }
        }
    }
    _socket = newsocket;
    _client = newsocket->get_client();
    _client->attatch(this);
    _lastip = _socket->get_ip_str();
    return true;
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
    for (udword_t i = 0; i < _charlist.size(); i++) {
        Char *chr = server.get_char(_charlist[i]);
        if (chr) {
            chr->remove();
            chr = nullptr;
        }
    }
    delete this;
}

Client* Account::get_client()
{
    Client *client = nullptr;
    if (_socket)
    {
        client = _socket->get_client();
    }
    return client;
}

bool Account::password_match(const std::string& pw)
{
    //Todo: MD5
    return _password == pw;
}
