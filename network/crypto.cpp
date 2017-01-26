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
#include "../core/config.h"
#include "../shell.h"

Crypto::Crypto() {
    _client_key_0 = 0;
    _client_key_1 = 0;
    _curr_key_0 = 0;
    _curr_key_1 = 0;
    _key = 0;
    _crypt_mode = CRYPTMODE_NONE;
}

#define N2L(C, LL) \
    LL  = ((C&0xff000000))>>24,\
    LL |= ((C&0x00ff0000))>>8,\
    LL |= ((C&0x0000ff00))<<8,\
    LL |= ((C&0x000000ff)<<24)

void Crypto::set_client_key(t_udword key) {
    //_key = key;
    N2L(key, _key);
    _curr_key_0 = (t_udword)((((~key) ^ 0x00001357) << 16) | ((key ^ 0xFFFFAAAA) & 0x0000FFFF));
    _curr_key_1 = (t_udword)(((key ^ 0x43210000) >> 16) | (((~key) ^ 0xABCDFFFF) & 0xFFFF0000));
}

void Crypto::set_mode_none() {
    _crypt_mode = CRYPTMODE_NONE;
}

void Crypto::set_mode_login() {
    _crypt_mode = CRYPTMODE_LOGIN;
}

void Crypto::set_mode_game() {
    _crypt_mode = CRYPTMODE_GAME;
}

void Crypto::detect_client_keys(t_byte * buffer, t_udword l) {
    t_udword length = (t_udword)toruscfg.crypto_keys.size();
    t_byte * temp_buffer = new t_byte[l];
    for (t_udword i = 0; i < length; i++) {
        memcpy(temp_buffer, buffer, l);
        _client_key_0 = toruscfg.crypto_keys[i].first;
        _client_key_1 = toruscfg.crypto_keys[i].second;
        _decrypt_loginmode(temp_buffer, l);
        if (temp_buffer[0] == 0x80 && temp_buffer[30] == 0x00 && temp_buffer[60] == 0x00) {
            TORUSSHELLECHO("Client keys [" << i << "] " << std::hex << _client_key_0 << ":" << _client_key_1);
            delete temp_buffer;
            return;
        }
    }
    TORUSSHELLECHO("ERROR! Client keys not found");
    _client_key_0 = 0;
    _client_key_1 = 0;
}

void Crypto::decrypt(t_byte * buffer, t_udword l) {
    switch(_crypt_mode) {
        case CRYPTMODE_NONE: return;
        case CRYPTMODE_LOGIN: {
            _decrypt_loginmode(buffer, l);
        } break;
        case CRYPTMODE_GAME: {
            _decrypt_gamemode(buffer, l);
        } break;
    }
}

void Crypto::crypt(t_byte * buffer, t_udword l) {
    switch(_crypt_mode) {
        case CRYPTMODE_NONE: return;
        case CRYPTMODE_LOGIN: {
            _crypt_loginmode(buffer, l);
        } break;
        case CRYPTMODE_GAME: {
            _crypt_gamemode(buffer, l);
        } break;
    }
}

void Crypto::_decrypt_loginmode(t_byte * buffer, t_udword l) {
    t_udword old_key_0, old_key_1;
    for(t_udword i = 0; i < l; i++)
    {
//        buffer[i] = buffer[i] ^ (t_byte) _curr_key_0;
//        old_key_0 = _curr_key_0;
//        old_key_1 = _curr_key_1;
//        _curr_key_0 = ((old_key_0 >> 1) | (old_key_1 << 31)) ^ _client_key_0;
//        _curr_key_1 = ((old_key_1 >> 1) | (old_key_0 << 31)) ^ _client_key_1;
        // Decrypt the byte:
        buffer[i] = (t_byte)_curr_key_0 ^ buffer[i];
        // Reset the keys:
        old_key_0 = _curr_key_0;
        old_key_1 = _curr_key_1;
        _curr_key_0 = (t_udword)(((old_key_0 >> 1) | (old_key_1 << 31)) ^ _client_key_0);
        old_key_1 = (((old_key_1 >> 1) | (old_key_0 << 31)) ^ _client_key_1);
        _curr_key_1 = ((old_key_1 >> 1) | (old_key_0 << 31)) ^ _client_key_1;

//        _curr_key_1 = (t_udword)((((((old_key_1 >> 1) | (old_key_0 << 31)) ^ (_client_key_1 - 1)) >> 1) | (old_key_0 << 31)) ^ _client_key_1);
    }
}

void Crypto::_decrypt_gamemode(t_byte * buffer, t_udword l) {

}

void Crypto::_crypt_loginmode(t_byte * buffer, t_udword l) {
    _decrypt_loginmode(buffer, l);
}

void Crypto::_crypt_gamemode(t_byte * buffer, t_udword l) {

}