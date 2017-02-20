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

#ifndef _TORUS_GAME_ACCOUNT_H
#define _TORUS_GAME_ACCOUNT_H

#include "../library/types.h"
#include <string>
#include <vector>

class Char;
class Socket;

enum PRIVLVL {
    PRIV_GUEST,
    PRIV_PLAYER,
    PRIV_COUNSELOR,
    PRIV_SEER,
    PRIV_GM,
    PRIV_DEV,
    PRIV_ADMIN
};

// Expansion Flags
#define EXP_LBR 0x001   ///< Default game.
#define EXP_T2A 0x002   ///< The Second Age.
#define EXP_AOS 0x004   ///< Age Of Shadows.
#define EXP_SE  0x008   ///< Samurai Empire.
#define EXP_ML  0x010   ///< Mondain's Legacy.
#define EXP_SA  0x020   ///< Stygian Abyss.
#define EXP_HS  0x040   ///< High Seas.

class Account {
private:
    std::string _name;    ///< Account name, used mostly for login.
    std::string _password;///< Account password.
    std::vector<Char *> _charlist;     ///< Character's list.
    t_udword _flags;        ///< Account flags.
    t_uword _expansion;     ///< Expansion level this account has access.
    std::string _lastip;  ///< Last IP connected to this account.
    Socket *_socket;        ///< Pointer to the socket currently connected to this account.
    PRIVLVL _privlevel;
public:
    Account(const t_byte *name, const t_byte *pw, t_udword flags = 0, t_uword exp = 0);
    ~Account();
    /**
    * @brief get the total count of characters this account has.
    *
    * @return the count of characters.
    */
    t_byte get_char_count();
    /**
    * @brief get the max amount of characters this account can have.
    *
    * @return the max amount.
    */
    t_byte get_max_chars();
    /**
    * @brief Checks if this account can have more characters.
    *
    * @return false if not.
    */
    bool can_add_char();
    /**
    * @brief adds a character to this account.
    *
    * @param chr the character to add.
    */
    bool add_char(Char *chr);
    /**
    * @brief remove a character from this account.
    *
    * @param chr the character to remove.
    */
    bool delete_char(Char *chr);
    /**
    * @brief Receiving a connection for this account.
    *
    * @param socket the socket connecting to the account.
    */
    void connect(Socket *socket);
    /**
    * @brief Gets the account's privilege level
    *
    * @return the PRIVLVL.
    */
    PRIVLVL get_privlevel();
    /**
    * @brief Asign a PRIVLVL for this account.
    *
    * @param lvl the new PRIVLVL.
    */
    void set_privlevel(PRIVLVL lvl);
    /**
    * @brief Remove this account.
    *
    * Remove this account and all the characters containing.
    */
    void remove();
};

#endif //_TORUS_GAME_ACCOUNT_H