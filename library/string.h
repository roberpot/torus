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

#ifndef __TORUS_STRING_H
#define __TORUS_STRING_H

#include <cstring>
#include <string>
#include <iomanip>
#include <ostream>
#include <vector>
#include <library/types.h>

#define ISWHITESPACE(X) (X == ' ' || X == '\t')
#define LSTRIP(X) while (ISWHITESPACE(X[0])) { X++; }
#define RSTRIP(X) \
{ \
    size_t l = strlen(X) - 1; \
    while (l && ISWHITESPACE(X[l])) { \
        X[l] = '\0';\
        l--; \
    } \
}

#define STRIP(X) { \
    LSTRIP(X); \
    RSTRIP(X); \
}

std::string print_hex_buffer(const t_byte * buffer, udword_t len);
std::string remove_prefix(std::string p, std::string s);
std::string remove_prefix(const t_byte * p, std::string s);
std::string remove_prefix(const t_byte * p, const t_byte * s);

template <typename T>
struct __HexHelperStruct {
    __HexHelperStruct(T x) { data.x = x; }
    union {
        T x;
        t_ubyte raw[sizeof(T)];
    } data;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& o, const __HexHelperStruct<T>& hs) {
    for(udword_t i = 0; i < sizeof(T); i++) {
        o << std::hex << std::setfill('0') <<  std::setw(2) << (int)hs.data.raw[i];
}
return o;
}

template <typename T>
inline __HexHelperStruct<T> hex(T x) {
    return __HexHelperStruct<T>(x);
}

std::string hex_dump_buffer(const t_byte * buffer, const udword_t size);

std::vector<std::string> split(const std::string &str, t_byte del);
std::string clean(const std::string &str);

#endif //__TORUS_STRING_H
