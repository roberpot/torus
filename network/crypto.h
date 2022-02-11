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

#include <library/types.h>

class Crypto {
public:
    Crypto();
    void set_client_seed(udword_t seed);
    void set_crypt_key(udword_t maskHi, udword_t masLo);
    void set_mode_none();
    void set_mode_login();
    void set_mode_game();
    void detect_client_keys(t_byte * buffer, udword_t l);
    void decrypt(t_byte * buffer, udword_t l);
    void crypt(t_ubyte * buffer, udword_t l);
private:
    enum class CryptMode {
        CRYPTMODE_NONE = 0,
        CRYPTMODE_LOGIN,
        CRYPTMODE_GAME
    };
    enum class CryptType {
        CRYPTTYPE_NONE,
        CRYPTTYPE_BLOWFISH,
        CRYPTTYPE_BLOWTWOFISH,
        CRYPTTYPE_TWOFISH,
        CRYPTTYPE_CLASSIC   //
    };

    void _decrypt_login_gt_0x125360(t_ubyte * buffer, udword_t l);
    void _decrypt_login_eq_0x125360(t_ubyte * buffer, udword_t l);
    void _decrypt_login_lt_0x125360(t_ubyte * buffer, udword_t l);
    void _crypt_login_gt_0x125360(t_ubyte * buffer, udword_t l);
    void _crypt_login_eq_0x125360(t_ubyte * buffer, udword_t l);
    void _crypt_login_lt_0x125360(t_ubyte * buffer, udword_t l);
    void (Crypto::*_decrypt_login_method)(t_ubyte *, udword_t);
    void (Crypto::*_crypt_login_method)(t_ubyte *, udword_t);
    void (Crypto::*_decrypt_game_method)(t_ubyte *, udword_t);
    void (Crypto::*_crypt_game_method)(t_ubyte *, udword_t);

    void _decrypt_loginmode(t_byte * buffer, udword_t l);
    void _decrypt_gamemode(t_byte * buffer, udword_t l);
    void _crypt_loginmode(t_byte * buffer, udword_t l);
    void _crypt_gamemode(t_byte * buffer, udword_t l);
    udword_t _client_key_lo;
    udword_t _client_key_hi;
    udword_t _curr_key_lo;
    udword_t _curr_key_hi;
    udword_t _seed;
    CryptMode _crypt_mode;
    CryptType _crypt_type;

public:
    inline bool has_encryption()
    {
        return _crypt_type != CryptType::CRYPTTYPE_NONE;
    }
};

#endif //__TORUS_CRYPTO_H
