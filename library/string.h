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
#include "../core/types.h"

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

std::string print_hex_buffer(const t_byte * buffer, t_udword len);
std::string remove_prefix(std::string p, std::string s);
std::string remove_prefix(const t_byte * p, std::string s);
std::string remove_prefix(const t_byte * p, const t_byte * s);

struct HexCharStruct
{
    unsigned char c;
    HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
    return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(unsigned char _c)
{
    return HexCharStruct(_c);
}

#endif //__TORUS_STRING_H
