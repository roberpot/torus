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

#ifndef __TORUS_CONFIG_H
#define __TORUS_CONFIG_H

#include <set>
#include <string>
#include <vector>
#include "types.h"

extern class Config {
public:
    Config();
    void load_config_file(const t_byte * file);
    // torus.ini config.
    std::string log_filename;
    std::string server_name;
    std::string server_desc;
    std::string torustable_file;
    t_byte * net_addr;
    t_word net_port;
    // extra config.
    std::set<std::string> obscene_strings;
    std::vector<t_dword> fame_ranges;
    std::vector<std::string> fame_titles;
    std::vector<t_dword> karma_ranges;
    std::vector<std::string> karma_titles;
    std::vector<t_dword> noto_karma_ranges;
    std::vector<t_dword> noto_fame_ranges;
    std::vector<std::string> noto_titles;
    std::vector<std::string> runes;
    std::vector<std::pair<t_udword, t_udword> > crypto_keys;
private:
    void _reset();
} toruscfg;

#endif //__TORUS_CONFIG_H
