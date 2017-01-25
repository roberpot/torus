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

#define ISWHITESPACE(X) (X == ' ' || X == '\t')
#define LSTRIP(X) while (ISWHITESPACE(X[0])) { X++; }
#define RSTRIP(X) \
{ \
    unsigned int l = strlen(X) - 1; \
    while (l && ISWHITESPACE(X[l])) { \
        X[l] = '\0';\
        l--; \
    } \
}

#define STRIP(X) { \
    LSTRIP(X); \
    RSTRIP(X); \
}

std::string print_hex_buffer(const char * buffer, unsigned int len);
std::string remove_prefix(std::string p, std::string s);
std::string remove_prefix(const char * p, std::string s);
std::string remove_prefix(const char * p, const char * s);

#endif //__TORUS_STRING_H
