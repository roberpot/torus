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

#ifndef __TORUS_GAME_CLIENTS_CLILOC_VARIANT_H_
#define __TORUS_GAME_CLIENTS_CLILOC_VARIANT_H_

#include <library/types.h>

#include <vector>
#include <string>

class ClilocVariant {
    enum class VariantType {
        NONE,
        INTEGER,
        STRING
    };
    union VariantValue {
        std::wstring _str;
        dword_t _val;
        VariantValue() :
            _str(L""),
            _val(0) { }
        VariantValue(const VariantValue& other) : _str(other._str), _val(other._val) {
        }
        VariantValue operator=(const VariantValue& other) {
            if (other._str.size()) {
                _str = other._str;
            } else {
                _val = other._val;
            }
            return *this;
        }
        ~VariantValue() {
        }
    };
    VariantType _type;
    VariantValue _value;
public:
    ClilocVariant();

    ClilocVariant(const std::wstring& str);
    ClilocVariant(const dword_t& val);
    ClilocVariant(const ClilocVariant& other);
    ClilocVariant operator=(const ClilocVariant& other);
    ~ClilocVariant();

    void add_arg(const std::wstring& str);
    void add_arg(const dword_t& val);
    bool is_type_integer();
    bool is_type_string();
    dword_t get_integer();
    std::wstring get_string();
};

#endif  // __TORUS_GAME_CLIENTS_CLILOC_VARIANT_H_
