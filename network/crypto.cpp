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
#include <cstddef>

#include "crypto.h"
#include "../debug/callstack.h"
#include "../core/config.h"
#include "../shell.h"


Crypto::Crypto() {
    ADDTOCALLSTACK();
    _client_key_lo = 0;
    _client_key_hi = 0;
    _curr_key_lo = 0;
    _curr_key_hi = 0;
    _seed = 0;
    _crypt_mode = CRYPTMODE_NONE;
    _crypt_type = CRYPTTYPE_NONE;
}

void Crypto::set_client_seed(t_udword seed) {
    ADDTOCALLSTACK();
    _seed = seed;
}

void Crypto::set_crypt_key(t_udword keyHi, t_udword keyLo)
{
    _curr_key_hi = keyHi;
    _curr_key_lo = keyLo;
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
    t_ubyte * temp_buffer = new t_ubyte[l];  // Login packet needs the buffer to be unsigned.
    t_udword key = _seed;

    // Decrypting functors for each encryption type (lower than client 125360, equal or greater).
    void (Crypto::*decrypt_login_methods[3])(t_ubyte *, t_udword);
    void (Crypto::*crypt_login_methods[3])(t_ubyte *, t_udword);
    decrypt_login_methods[0] = &Crypto::_decrypt_login_gt_0x125360;
    decrypt_login_methods[1] = &Crypto::_decrypt_login_eq_0x125360;
    decrypt_login_methods[2] = &Crypto::_decrypt_login_lt_0x125360;
    crypt_login_methods[0] = &Crypto::_crypt_login_gt_0x125360;
    crypt_login_methods[1] = &Crypto::_crypt_login_eq_0x125360;
    crypt_login_methods[2] = &Crypto::_crypt_login_lt_0x125360;
    //

    // Converting the seed to the crypted key values
    t_udword m_tmp_CryptKeyLo = (((~_seed) ^ 0x00001357) << 16) | (((_seed) ^ 0xffffaaaa) & 0x0000ffff);
    t_udword m_tmp_CryptKeyHi = (((_seed) ^ 0x43210000) >> 16) | (((~_seed) ^ 0xabcdffff) & 0xffff0000);
    set_crypt_key(m_tmp_CryptKeyHi, m_tmp_CryptKeyLo);

    #ifdef CRYPTO_LOGIN_DEBUG
        TORUSSHELLECHO("Client keys base = 0x" << std::hex << m_tmp_CryptKeyLo << ", 0x" << m_tmp_CryptKeyHi << ", for seed " << _seed);
    #endif

    // Looping through the client crypt keys to detect the one matching the connecting client.
    for (t_udword i = 0; i < length; i++)
    {
        for (unsigned int j = 0; j < 3; j++) // Looping through the three decrypting functors to detect the one we need.
        {
            // the following three lines are required in each iteration to default the values for the crypt checks:
            memcpy(temp_buffer, buffer, sizeof(t_byte) * l);    // returning tmp_buffer to a copy of buffer to begin all the checks over.
            set_client_seed(key);   // returning the seed to default
            set_crypt_key(m_tmp_CryptKeyHi, m_tmp_CryptKeyLo);  // returning crypt keys to default

            // Early check, unencrypted clients already have the values we want without further processing.
            if (temp_buffer[0] == 0x80 && temp_buffer[30] == 0x00 && temp_buffer[60] == 0x00)
            {
                TORUSSHELLECHO("Unencrypted client connecting!!!");
                return;
            }

            // Setting the client keys to the respective keys in the table.
            _client_key_lo = toruscfg.crypto_keys[i].second;
            _client_key_hi = toruscfg.crypto_keys[i].first;

            // Calling the functor for this loop to compare and know what type of decrypt we need
            (this->*decrypt_login_methods[j])(temp_buffer, l);

            // Everything set and run, checking the resulting buffer for a valid decryption to begin the log in process.
            if (temp_buffer[0] == 0x80 && temp_buffer[30] == 0x00 && temp_buffer[60] == 0x00) {
                #ifdef CRYPTO_LOGIN_DEBUG
                    TORUSSHELLECHO("Client keys found [" << i << "][" << 0 << "] " << std::hex << _client_key_lo << ":" << _client_key_hi);
                #endif
                delete temp_buffer;
                _decrypt_login_method = decrypt_login_methods[j];
                _crypt_login_method = crypt_login_methods[j];
                _client_key_lo = toruscfg.crypto_keys[i].second;
                _client_key_hi = toruscfg.crypto_keys[i].first;
                return;
            }
        }
    }
    TORUSSHELLECHO("ERROR! Client keys not found");
    _client_key_lo = 0;
    _client_key_hi = 0;
}

void Crypto::decrypt(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    t_ubyte *transictionBuffer = new t_ubyte[255];  // Login packet needs the buffer to be unsigned.
    memcpy(transictionBuffer, buffer, l);
    switch(_crypt_mode) {
        case CRYPTMODE_NONE: return;
        case CRYPTMODE_LOGIN: {
            (this->*_decrypt_login_method)(transictionBuffer, l);
        } break;
        case CRYPTMODE_GAME: {
            (this->*_decrypt_game_method)(transictionBuffer, l);
        } break;
    }
}

void Crypto::crypt(t_ubyte * buffer, t_udword l) {
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

void Crypto::_decrypt_login_gt_0x125360(t_ubyte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    t_udword old_key_lo, old_key_hi; 
    #ifdef CRYPTO_LOGIN_DEBUG
        TORUSSHELLECHO("Searching keys, lenght = " << l);
    #endif
    for(t_udword i = 0; i < l; ++i) {
        // Decrypt the byte:
        #ifdef CRYPTO_LOGIN_DEBUG
            std::stringstream ss;
            ss << "buffer[" << i << "] == " << std::to_string((t_ubyte)buffer[i]) << " ^ 0x" << std::hex << _curr_key_lo << "[";
        #endif
            buffer[i] = (int)buffer[i] ^ _curr_key_lo;
        #ifdef CRYPTO_LOGIN_DEBUG
            ss << std::dec << (unsigned int)buffer[i] << "]";
            TORUSSHELLECHO(ss.str());
        #endif
        // Reset the keys:
        old_key_lo = _curr_key_lo;
        old_key_hi = _curr_key_hi;
        // Update the keys:
        _curr_key_lo = ((old_key_lo >> 1) | (old_key_hi << 31)) ^ _client_key_lo;
        old_key_hi = ((old_key_hi >> 1) | (old_key_lo  << 31)) ^ _client_key_hi;
        _curr_key_hi = ((old_key_hi >> 1) | (old_key_lo << 31)) ^ _client_key_hi;
    }
}

void Crypto::_decrypt_login_eq_0x125360(t_ubyte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    t_udword old_key_0, old_key_1;
    for(t_udword i = 0; i < l; i++) {
        // Decrypt the byte:
        buffer[i] = (t_byte)(_curr_key_lo ^ buffer[i]);
        // Reset the keys:
        old_key_0 = _curr_key_lo;
        old_key_1 = _curr_key_hi;
        // Update the keys:
        _curr_key_hi =
                (_client_key_hi >> ((5 * old_key_1 * old_key_1) & 0xff))
                + (old_key_1 * _client_key_hi)
                + (old_key_0 * old_key_0 * 0x35ce9581)
                + 0x07afcc37;
        _curr_key_lo =
                (_client_key_lo >> ((3 * old_key_0 * old_key_0) & 0xff))
                + (old_key_0 * old_key_0)
                - (_curr_key_hi * _curr_key_hi * 0x4c3a1353)
                + 0x16ef783f;
    }
}

void Crypto::_decrypt_login_lt_0x125360(t_ubyte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    t_udword old_key_0, old_key_1;
    for(t_udword i = 0; i < l; i++) {
        // Decrypt the byte:
        buffer[i] = (t_byte)(_curr_key_lo ^ buffer[i]);
        // Reset the keys:
        old_key_0 = _curr_key_lo;
        old_key_1 = _curr_key_hi;
        // Update the keys:
        _curr_key_lo = ((old_key_0 >> 1) | (old_key_1 << 31)) ^ _client_key_lo;
        _curr_key_hi = ((old_key_1 >> 1) | (old_key_0 << 31)) ^ _client_key_hi;
    }
}


void Crypto::_crypt_login_gt_0x125360(t_ubyte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    _decrypt_login_gt_0x125360(buffer, l);
}

void Crypto::_crypt_login_eq_0x125360(t_ubyte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    _decrypt_login_eq_0x125360(buffer, l);
}

void Crypto::_crypt_login_lt_0x125360(t_ubyte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    _decrypt_login_lt_0x125360(buffer, l);
}

void Crypto::_decrypt_gamemode(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(l);
}

void Crypto::_crypt_gamemode(t_byte * buffer, t_udword l) {
    ADDTOCALLSTACK();
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(l);
}