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

#include <iomanip>
#include <sstream>
#include "string.h"

std::string print_hex_buffer(const char * buffer, unsigned int len) {
    std::stringstream s;
    s << std::hex;
    for (unsigned int i = 0; i < len; i++) {
        s << ":"  << std::setfill('0') << std::setw(2) << (int)(unsigned char)buffer[i];
    }
    return s.str();
}

std::string remove_prefix(std::string p, std::string s) {
    return remove_prefix(p.c_str(), s);
}

std::string remove_prefix(const char * p, std::string s) {
    unsigned int n = 0, l = strlen(p);
    char * c = (char *)s.c_str();
    while (n < l && *c == *p) {
        c++;
        p++;
        n++;
    }
    std::string r(c);
    return r;
}

std::string remove_prefix(const char * p, const char * s) {
    std::string ss(s);
    return remove_prefix(p, ss);
}