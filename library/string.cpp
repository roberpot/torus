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
#include <library/string.h>

std::string print_hex_buffer(const t_byte * buffer, udword_t len) {
    std::stringstream s;
    s << std::hex;
    for (udword_t i = 0; i < len; i++) {
        s << ":"  << std::setfill('0') << std::setw(2) << (dword_t)(udword_t)buffer[i];
    }
    return s.str();
}

std::string remove_prefix(std::string p, std::string s) {
    return remove_prefix(p.c_str(), s);
}

std::string remove_prefix(const t_byte * p, std::string s) {
    size_t n = 0, l = strlen(p);
    t_byte * c = (char *)s.c_str();
    while (n < l && *c == *p) {
        c++;
        p++;
        n++;
    }
    std::string r(c);
    return r;
}

std::string remove_prefix(const t_byte * p, const t_byte * s) {
    std::string ss(s);
    return remove_prefix(p, ss);
}

std::string hex_dump_buffer(const t_byte * buffer, const udword_t size) {
    std::stringstream s;
    udword_t lines = size / 10;
    udword_t current_line_size;
    if (size % 10) lines++;
    s << " ## | 00 01 02 03 04 05 06 07 08 09 | *RAW**RAW* |" << std::endl;
    s << "----|-------------------------------|------------|" << std::endl;
    for(udword_t current_line = 0; current_line < lines; ++current_line) {
        s << " " << std::setfill(' ') << std::setw(2) << current_line << " | ";
        current_line_size = 10;
        if (current_line == (lines - 1) && size % 10) current_line_size = size % 10;
        for(udword_t curr_byte = 0; curr_byte < current_line_size; ++curr_byte) {
            s << hex(buffer[10 * current_line + curr_byte]) << " ";
        }
        if (current_line == (lines - 1) && size % 10) {
            for (udword_t extra = size % 10; extra < 10; ++extra) {
                s << "   ";
            }
        }
        s << "| ";
        for(udword_t curr_byte = 0; curr_byte < current_line_size; ++curr_byte) {
            s << buffer[10 * current_line + curr_byte];
        }
        if (current_line == (lines - 1) && size % 10) {
            for (udword_t extra = size % 10; extra < 10; ++extra) {
                s << " ";
            }
        }
        s << " |" << std::endl;
    }
    return s.str();
}
