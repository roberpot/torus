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

#ifndef __TORUS_CRYPTO_H
#define __TORUS_CRYPTO_H

#include "../core/types.h"

class Crypto {
public:
    Crypto();
    void set_client_key(UDWORD key);
    void set_mode_none();
    void set_mode_login();
    void set_mode_game();
    void detect_client_keys(char * buffer, unsigned int l);
    void decrypt(char * buffer, unsigned int l);
    void crypt(char * buffer, unsigned int l);
private:
    void _decrypt_loginmode(char * buffer, unsigned int l);
    void _decrypt_gamemode(char * buffer, unsigned int l);
    void _crypt_loginmode(char * buffer, unsigned int l);
    void _crypt_gamemode(char * buffer, unsigned int l);
    UDWORD _client_key_0;
    UDWORD _client_key_1;
    UDWORD _curr_key_0;
    UDWORD _curr_key_1;
    UDWORD _key;
    enum {
        CRYPTMODE_NONE = 0,
        CRYPTMODE_LOGIN,
        CRYPTMODE_GAME
    } _crypt_mode;
};

#endif //__TORUS_CRYPTO_H
