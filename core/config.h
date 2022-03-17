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
#include <library/types.h>
#include <library/vector.h>

#define BASE_MAP_MAX 5  ///< Max amount of Base Maps we can handle.
#define VIRTUAL_MAP_MAX TUBYTE_MAX  ///< Max amount of Virtual Maps we can handle.

struct MapFile {
    uword_t x;
    uword_t y;
    t_ubyte sector_size;
    t_ubyte id;
    std::string filename;
    MapFile(t_ubyte mid, uword_t mx, uword_t my, t_ubyte ss, std::string mfn) {
        id = mid;
        x = mx;
        y = my;
        sector_size = ss;
        filename = mfn;
    }
};

struct ServerInfo {
    std::string name;
    std::string ip;
    uword_t port;
    ServerInfo(std::string &n, std::string &i, uword_t &p){
        name = n;
        ip = i;
        port = p;
    }
    ServerInfo(){
        name = "";
        ip = "0.0.0.0";
        port = 0;
    }
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
    ServerInfo login_server;
    std::vector<ServerInfo> _game_servers;
    udword_t gameserver_index;
    uword_t tick_duration;
    uword_t tick_duration_overloaded;
    uword_t tick_duration_idle;
    // file cfg.
    std::string file_path;
    // extra config.
    std::set<std::string> obscene_strings;
    ttl::vector<dword_t> fame_ranges;
    ttl::vector<std::string> fame_titles;
    ttl::vector<dword_t> karma_ranges;
    ttl::vector<std::string> karma_titles;
    ttl::vector<dword_t> noto_karma_ranges;
    ttl::vector<dword_t> noto_fame_ranges;
    ttl::vector<std::string> noto_titles;
    ttl::vector<std::string> runes;
    std::vector<std::pair<udword_t, udword_t> > crypto_keys;
    // Maps.
    std::vector<MapFile> map_files;
    std::vector<std::pair<t_ubyte, t_ubyte>> maps;  //VirtualMapIndex, MapFileId
    uword_t buffer_size;

    void add_game_server(ServerInfo &server);
    void add_game_server(std::string& name, std::string& ip, uword_t& port);
private:
    void _reset();
} toruscfg;

#endif //__TORUS_CONFIG_H
