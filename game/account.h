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

#ifndef __TORUS_GAME_ACCOUNT_H
#define __TORUS_GAME_ACCOUNT_H

#include <string>
#include <vector>
#include <library/system_headers.h>
#include <library/types.h>

class Char;
class Socket;
class Uid;

enum PRIVLVL
{
    PRIV_GUEST,
    PRIV_PLAYER,
    PRIV_COUNSELOR,
    PRIV_SEER,
    PRIV_GM,
    PRIV_DEV,
    PRIV_ADMIN,
    PRIV_QTY
};

// Expansion Flags
#define EXP_LBR 0x001   ///< Default game.
#define EXP_T2A 0x002   ///< The Second Age.
#define EXP_AOS 0x004   ///< Age Of Shadows.
#define EXP_SE  0x008   ///< Samurai Empire.
#define EXP_ML  0x010   ///< Mondain's Legacy.
#define EXP_SA  0x020   ///< Stygian Abyss.
#define EXP_HS  0x040   ///< High Seas.

class Client;

class Account {
private:
    std::string _name;      ///< Account name, used mostly for login.
    std::string _password;  ///< Account password.
    std::vector<Uid> _charlist;  ///< Character's list.
    Char *_character;
    udword_t _flags;        ///< Account flags.
    uword_t _expansion;     ///< Expansion level this account has access.
    std::string _lastip;    ///< Last IP connected to this account.
    Socket *_socket;        ///< Pointer to the socket currently connected to this account.
    PRIVLVL _privlevel;
    Client *_client;
public:
    Account();
    ~Account();
    Account(std::string accname, std::string accpw, PRIVLVL accpriv);
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
    * @brief Get the character at the given slot.
    *
    * @param slot The character slot.
    * @return The Char, if exists.
    */
    Char* get_char(const udword_t &slot);
    /**
    * @brief remove a character from this account.
    *
    * @param chr the character to remove.
    */
    bool remove_char(Char *chr);
    /**
    * @brief Receiving a connection for this account.
    *
    * @param socket the socket connecting to the account.
    */
    bool connect(Socket *socket);
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
    /**
     * @brief Get the Client attached to this account, if any.
     * 
     * @return the Client or nullptr.
     */
    Client *get_client();
    /**
     * @brief Check if the account's password match the given one.
     * 
     * @param pw The password to check.
     * @return True if both passwords match, false otherwise.
     */
    bool password_match(const std::string &pw);
};

#endif // __TORUS_GAME_ACCOUNT_H