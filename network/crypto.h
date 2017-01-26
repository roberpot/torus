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
    void set_client_key(t_udword key);
    void set_mode_none();
    void set_mode_login();
    void set_mode_game();
    void detect_client_keys(t_byte * buffer, t_udword l);
    void decrypt(t_byte * buffer, t_udword l);
    void crypt(t_byte * buffer, t_udword l);
private:
    void _decrypt_loginmode(t_byte * buffer, t_udword l);
    void _decrypt_gamemode(t_byte * buffer, t_udword l);
    void _crypt_loginmode(t_byte * buffer, t_udword l);
    void _crypt_gamemode(t_byte * buffer, t_udword l);
    t_udword _client_key_0;
    t_udword _client_key_1;
    t_udword _curr_key_0;
    t_udword _curr_key_1;
    t_udword _key;
    enum {
        CRYPTMODE_NONE = 0,
        CRYPTMODE_LOGIN,
        CRYPTMODE_GAME
    } _crypt_mode;
};

#endif //__TORUS_CRYPTO_H
