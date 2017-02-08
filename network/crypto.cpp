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

#include <cstring>

#include "crypto.h"
#include "../debug/callstack.h"
#include "../core/config.h"
#include "../shell.h"

Crypto::Crypto() {
    ADDTOCALLSTACK();
    _client_key_0 = 0;
    _client_key_1 = 0;
    _curr_key_0 = 0;
    _curr_key_1 = 0;
    _seed = 0;
    _crypt_mode = CRYPTMODE_NONE;
}

void Crypto::set_client_seed(t_udword seed) {
    ADDTOCALLSTACK();
    _seed = seed;
    _curr_key_0 = (t_udword)((((~_seed) ^ 0x00001357) << 16) | ((_seed ^ 0xFFFFAAAA) & 0x0000FFFF));
    _curr_key_1 = (t_udword)(((_seed ^ 0x43210000) >> 16) | (((~_seed) ^ 0xABCDFFFF) & 0xFFFF0000));
}

void Crypto::set_mode_none() {
    ADDTOCALLSTACK();
    _crypt_mode = CRYPTMODE_NONE;
}

void Crypto::set_mode_login() {
    ADDTOCALLSTACK();
    _crypt_mode = CRYPTMODE_LOGIN;
}

void Crypto::set_mode_game() {
    ADDTOCALLSTACK();
    _crypt_mode = CRYPTMODE_GAME;
}

void Crypto::detect_client_keys(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    t_udword length = (t_udword)toruscfg.crypto_keys.size();
    t_byte * temp_buffer = new t_byte[l];
    t_udword key = _seed;
    void (Crypto::*decrypt_login_methods[3])(t_byte *, t_udword);
    void (Crypto::*crypt_login_methods[3])(t_byte *, t_udword);
    decrypt_login_methods[0] = &Crypto::_decrypt_login_gt_0x125360;
    decrypt_login_methods[1] = &Crypto::_decrypt_login_eq_0x125360;
    decrypt_login_methods[2] = &Crypto::_decrypt_login_lt_0x125360;
    crypt_login_methods[0] = &Crypto::_crypt_login_gt_0x125360;
    crypt_login_methods[1] = &Crypto::_crypt_login_eq_0x125360;
    crypt_login_methods[2] = &Crypto::_crypt_login_lt_0x125360;
    for (t_udword i = 0; i < length; i++) {
        for (unsigned int j = 0; j < 3; j++) {
            memcpy(temp_buffer, buffer, sizeof(t_byte) * l);
            set_client_seed(key);
            if (temp_buffer[0] == 0x80 && temp_buffer[30] == 0x00 && temp_buffer[60] == 0x00) {
                TORUSSHELLECHO("UNENCRYPTED!!!");
                return;
            }
            _client_key_0 = toruscfg.crypto_keys[i].first;
            _client_key_1 = toruscfg.crypto_keys[i].second;
            (this->*decrypt_login_methods[j])(temp_buffer, l);
            if (temp_buffer[0] == 0x80 && temp_buffer[30] == 0x00 && temp_buffer[60] == 0x00) {
                TORUSSHELLECHO("Client keys [" << i << "][" << j << "] " << std::hex << _client_key_0 << ":" << _client_key_1);
                delete temp_buffer;
                _decrypt_login_method = decrypt_login_methods[j];
                _crypt_login_method = crypt_login_methods[j];
                _client_key_0 = toruscfg.crypto_keys[i].first;
                _client_key_1 = toruscfg.crypto_keys[i].second;
                return;
            }
        }
    }
    TORUSSHELLECHO("ERROR! Client keys not found");
    _client_key_0 = 0;
    _client_key_1 = 0;
}

void Crypto::decrypt(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    switch(_crypt_mode) {
        case CRYPTMODE_NONE: return;
        case CRYPTMODE_LOGIN: {
            (this->*_decrypt_login_method)(buffer, l);
        } break;
        case CRYPTMODE_GAME: {
            (this->*_decrypt_game_method)(buffer, l);
        } break;
    }
}

void Crypto::crypt(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    switch(_crypt_mode) {
        case CRYPTMODE_NONE: return;
        case CRYPTMODE_LOGIN: {
            (this->*_crypt_login_method)(buffer, l);
        } break;
        case CRYPTMODE_GAME: {
            (this->*_crypt_game_method)(buffer, l);
        } break;
    }
}

void Crypto::_decrypt_login_gt_0x125360(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    t_udword old_key_0, old_key_1;
    for(t_udword i = 0; i < l; i++) {
        // Decrypt the byte:
        buffer[i] = (t_byte)(_curr_key_0 ^ buffer[i]);
        // Reset the keys:
        old_key_0 = _curr_key_0;
        old_key_1 = _curr_key_1;
        // Update the keys:
        _curr_key_0 = ((old_key_0 >> 1) | (old_key_1 << 31)) ^ _client_key_0;
        old_key_1 = ((old_key_1 >> 1) | (old_key_0  << 31)) ^ _client_key_1;
        _curr_key_1 = ((old_key_1 >> 1) | (old_key_0 << 31)) ^ _client_key_1;
    }
}

void Crypto::_decrypt_login_eq_0x125360(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    t_udword old_key_0, old_key_1;
    for(t_udword i = 0; i < l; i++) {
        // Decrypt the byte:
        buffer[i] = (t_byte)(_curr_key_0 ^ buffer[i]);
        // Reset the keys:
        old_key_0 = _curr_key_0;
        old_key_1 = _curr_key_1;
        // Update the keys:
        _curr_key_1 =
                (_client_key_1 >> ((5 * old_key_1 * old_key_1) & 0xff))
                + (old_key_1 * _client_key_1)
                + (old_key_0 * old_key_0 * 0x35ce9581)
                + 0x07afcc37;
        _curr_key_0 =
                (_client_key_0 >> ((3 * old_key_0 * old_key_0) & 0xff))
                + (old_key_0 * old_key_0)
                - (_curr_key_1 * _curr_key_1 * 0x4c3a1353)
                + 0x16ef783f;
    }
}

void Crypto::_decrypt_login_lt_0x125360(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    t_udword old_key_0, old_key_1;
    for(t_udword i = 0; i < l; i++) {
        // Decrypt the byte:
        buffer[i] = (t_byte)(_curr_key_0 ^ buffer[i]);
        // Reset the keys:
        old_key_0 = _curr_key_0;
        old_key_1 = _curr_key_1;
        // Update the keys:
        _curr_key_0 = ((old_key_0 >> 1) | (old_key_1 << 31)) ^ _client_key_0;
        _curr_key_1 = ((old_key_1 >> 1) | (old_key_0 << 31)) ^ _client_key_1;
    }
}


void Crypto::_crypt_login_gt_0x125360(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    _decrypt_login_gt_0x125360(buffer, l);
}

void Crypto::_crypt_login_eq_0x125360(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    _decrypt_login_eq_0x125360(buffer, l);
}

void Crypto::_crypt_login_lt_0x125360(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    _decrypt_login_lt_0x125360(buffer, l);
}

void Crypto::_decrypt_gamemode(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();

}

void Crypto::_crypt_gamemode(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();

}