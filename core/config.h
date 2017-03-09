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
#include "../library/types.h"
#include "../library/vector.h"

#define BASE_MAP_MAX 5  ///< Max amount of Base Maps we can handle.
#define VIRTUAL_MAP_MAX TUBYTE_MAX  ///< Max amount of Virtual Maps we can handle.

struct MapFile {
    t_uword x;
    t_uword y;
    t_ubyte sector_size;
    t_ubyte id;
    std::string filename;
    MapFile(t_ubyte mid, t_uword mx, t_uword my, t_ubyte ss, std::string mfn) {
        id = mid;
        x = mx;
        y = my;
        sector_size = ss;
        filename = mfn;
    }
};

struct DBSettings {
    std::string _db_host;
    std::string _db_name;
    std::string _db_user;
    std::string _db_password;
    std::string _table_accounts;
};

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
    t_uword tick_duration;
    t_uword tick_duration_overloaded;
    t_uword tick_duration_idle;
    // file cfg.
    std::string file_path;
    // extra config.
    std::set<std::string> obscene_strings;
    ttl::vector<t_dword> fame_ranges;
    ttl::vector<std::string> fame_titles;
    ttl::vector<t_dword> karma_ranges;
    ttl::vector<std::string> karma_titles;
    ttl::vector<t_dword> noto_karma_ranges;
    ttl::vector<t_dword> noto_fame_ranges;
    ttl::vector<std::string> noto_titles;
    ttl::vector<std::string> runes;
    std::vector<std::pair<t_udword, t_udword> > crypto_keys;
    // Maps.
    std::vector<MapFile> map_files;
    std::vector<std::pair<t_ubyte, t_ubyte>> maps;
    // DataBase.
    DBSettings _dbsettings;
private:
    void _reset();
} toruscfg;

#endif //__TORUS_CONFIG_H
