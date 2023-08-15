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
#include <codecvt>
#include <locale>
#include <library/string.h>
#include <algorithm>
#include <cctype>
#include <cwctype>

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

std::string hex_dump_buffer(const uint8_t* buffer, const udword_t size) {
    std::stringstream s;
    udword_t line_size = 16;
    udword_t lines = size / line_size;
    udword_t current_line_size;
    if (size % line_size) lines++;
    s << " ## | 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 |                   *RAW**RAW*                    |" << std::endl;
    s << "----|-------------------------------------------------|-------------------------------------------------|" << std::endl;
    for (size_t i = 1; i <= size; ++i)
    {
        s << hex(buffer[i - 1]) << " ";
        if (i % line_size == 0 )
        {
            s << std::endl;
        }
    }
    return s.str();
    for(udword_t current_line = 0; current_line < lines; ++current_line) {
        s << " " << std::setfill(' ') << std::setw(2) << current_line << " | ";
        current_line_size = line_size;
        if (current_line == (lines - 1) && size % line_size) current_line_size = size % line_size;
        for (udword_t curr_byte = 0; curr_byte < current_line_size; ++curr_byte) {
            s << hex(buffer[line_size * current_line + curr_byte]) << " ";
        }
        if (current_line == (lines - 1) && size % line_size) {
            for (udword_t extra = size % line_size; extra < line_size; ++extra) {
                s << "   ";
            }
        }
        /*s << "| ";
        for (udword_t curr_byte = 0; curr_byte < current_line_size; ++curr_byte) {
            s << buffer[line_size * current_line + curr_byte];
        }
        if (current_line == (lines - 1) && size % line_size) {
            for (udword_t extra = size % line_size; extra < line_size; ++extra) {
                s << " ";
            }
        }
        s << " |" << std::endl;*/
    }
    return s.str();
}

std::vector<std::string> split(const std::string &str, t_byte del) {
    std::vector<std::string> ret;

    std::string::size_type prev_pos = 0;
    std::string::size_type pos = 0;
    while((pos = str.find(del, pos)) != std::string::npos)
    {
        std::string substring( str.substr(prev_pos, pos - prev_pos) );
        ret.push_back(substring);
        prev_pos = ++pos;
    }

    ret.push_back(str.substr(prev_pos, pos - prev_pos)); // Last word
    return ret;
}

std::vector<std::wstring> split(const std::wstring& wstr, t_byte del) {
    std::vector<std::wstring> ret;

    std::wstring::size_type prev_pos = 0;
    std::wstring::size_type pos = 0;
    while ((pos = wstr.find(del, pos)) != std::wstring::npos) {
        std::wstring substring(wstr.substr(prev_pos, pos - prev_pos));
        ret.push_back(substring);
        prev_pos = ++pos;
    }
    ret.push_back(wstr.substr(prev_pos, pos - prev_pos)); // Last word
    return ret;
}

std::string clean(const std::string &str) {
    std::string ret;
    size_t i = 0;
    for (; i < str.size(); ++i)
    {
        t_byte chr = str[i];
        if (chr == 92 || chr == 32 || chr == 34 || chr == '\0' || chr == '\r' || chr == '\t')
        {
            continue;
        }
        ret.push_back(str[i]);
    }
    return ret;
}

std::wstring clean(const std::wstring& str) {
    std::wstring ret;
    size_t i = 0;
    for (; i < str.size(); ++i) {
        wchar_t chr = str[i];
        if (chr == 92 || chr == 32 || chr == 34 || chr == '\0') {
            continue;
        }
        ret.push_back(str[i]);
    }
    return ret;
}

std::wstring to_wstring(const std::string& str) {
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
}


size_t find_table(const std::wstring* table, const std::wstring& elem) {
    size_t pos = 0;
    bool found = false;
    for (; pos < table->size(); ++pos) {
        if (std::wcscmp(table[pos].c_str(), elem.c_str()) == 0) {
            found = true;
            break;
        }
    }
    if (!found) {
        pos = SIZE_MAX;
    }
    return pos;
}