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

#include <cstring>
#include <fstream>
#include <shell.h>

#include <core/config.h>
#include <debug_support/info.h>
#include <library/string.h>


Config toruscfg;

Config::Config() {
}

void Config::load_config_file(const t_byte * file) {
    std::ifstream f(file);

    _reset();
    crypto_keys.push_back(std::make_pair(0x3F1E813D, 0xAB8A127F)); // 7.0.76
    crypto_keys.push_back(std::make_pair(0x2042036D, 0xA5D1BE7F)); // 7.0.73;
    crypto_keys.push_back(std::make_pair(0x207B217D, 0xA5C7527F)); // 7.0.72;
    crypto_keys.push_back(std::make_pair(0x203CC38D, 0xA5FDFE7F)); // 7.0.71;
    crypto_keys.push_back(std::make_pair(0x20E5E99D, 0xA598227F)); // 7.0.70;
    crypto_keys.push_back(std::make_pair(0x21C1A9DD, 0xA521227F)); // 7.0.66;
    crypto_keys.push_back(std::make_pair(0x2339EE5D, 0xA41FA27F)); // 7.0.580;
    crypto_keys.push_back(std::make_pair(0x23F2C46D, 0xA47E3E7F)); // 7.0.570;
    crypto_keys.push_back(std::make_pair(0x29cd362d, 0xa1d59e7f)); // 7.00.290
    crypto_keys.push_back(std::make_pair(0x29b5843d, 0xa1ea127f)); // 7.00.280
    crypto_keys.push_back(std::make_pair(0x2a7e164d, 0xa0081e7f)); // 7.00.270
    crypto_keys.push_back(std::make_pair(0x2a26ec5d, 0xa019a27f)); // 7.00.260
    crypto_keys.push_back(std::make_pair(0x2aef466d, 0xa07f3e7f)); // 7.00.250
    crypto_keys.push_back(std::make_pair(0x2ad7247d, 0xa065527f)); // 7.00.240
    crypto_keys.push_back(std::make_pair(0x2a9f868d, 0xa0437e7f)); // 7.00.230
    crypto_keys.push_back(std::make_pair(0x2b406c9d, 0xa0a1227f)); // 7.00.220
    crypto_keys.push_back(std::make_pair(0x2b08d6ad, 0xa0875e7f)); // 7.00.210
    crypto_keys.push_back(std::make_pair(0x2bf084bd, 0xa0fd127f)); // 7.00.200
    crypto_keys.push_back(std::make_pair(0x2bb976cd, 0xa0dbde7f)); // 7.00.190
    crypto_keys.push_back(std::make_pair(0x2c612cdd, 0xa328227f)); // 7.00.180
    crypto_keys.push_back(std::make_pair(0x2c29e6ed, 0xa30efe7f)); // 7.00.170
    crypto_keys.push_back(std::make_pair(0x2c11a4fd, 0xa313527f)); // 7.00.160
    crypto_keys.push_back(std::make_pair(0x2cda670d, 0xa3723e7f)); // 7.00.150
    crypto_keys.push_back(std::make_pair(0x2c822d1d, 0xa35da27f)); // 7.00.140
    crypto_keys.push_back(std::make_pair(0x2d4af72d, 0xa3b71e7f)); // 7.00.130
    crypto_keys.push_back(std::make_pair(0x2d32853d, 0xa38a127f)); // 7.00.120
    crypto_keys.push_back(std::make_pair(0x2dfb574d, 0xa3ed9e7f)); // 7.00.110
    crypto_keys.push_back(std::make_pair(0x2da36d5d, 0xa3c0a27f)); // 7.00.100
    crypto_keys.push_back(std::make_pair(0x2e6b076d, 0xa223be7f)); // 7.00.90
    crypto_keys.push_back(std::make_pair(0x2e53257d, 0xa23f527f)); // 7.00.80
    crypto_keys.push_back(std::make_pair(0x2e1bc78d, 0xa21bfe7f)); // 7.00.70
    crypto_keys.push_back(std::make_pair(0x2ec3ed9d, 0xa274227f)); // 7.00.60
    crypto_keys.push_back(std::make_pair(0x2e8b97ad, 0xa250de7f)); // 7.00.50
    crypto_keys.push_back(std::make_pair(0x2f7385bd, 0xa2ad127f)); // 7.00.40
    crypto_keys.push_back(std::make_pair(0x2f3bb7cd, 0xa2895e7f)); // 7.00.30
    crypto_keys.push_back(std::make_pair(0x2fe3addd, 0xa2e5227f)); // 7.00.20
    crypto_keys.push_back(std::make_pair(0x2faba7ed, 0xa2c17e7f)); // 7.00.10
    crypto_keys.push_back(std::make_pair(0x2f93a5fd, 0xa2dd527f)); // 7.00.00
    crypto_keys.push_back(std::make_pair(0x2c022d1d, 0xa31da27f)); // 6.00.140
    crypto_keys.push_back(std::make_pair(0x2dcaf72d, 0xa3f71e7f)); // 6.00.130
    crypto_keys.push_back(std::make_pair(0x2db2853d, 0xa3ca127f)); // 6.00.120
    crypto_keys.push_back(std::make_pair(0x2d7b574d, 0xa3ad9e7f)); // 6.00.110
    crypto_keys.push_back(std::make_pair(0x2d236d5d, 0xa380a27f)); // 6.00.100
    crypto_keys.push_back(std::make_pair(0x2eeb076d, 0xa263be7f)); // 6.00.90
    crypto_keys.push_back(std::make_pair(0x2ed3257d, 0xa27f527f)); // 6.00.80
    crypto_keys.push_back(std::make_pair(0x2e9bc78d, 0xa25bfe7f)); // 6.00.70
    crypto_keys.push_back(std::make_pair(0x2e43ed9d, 0xa234227f)); // 6.00.60
    crypto_keys.push_back(std::make_pair(0x2e0b97ad, 0xa210de7f)); // 6.00.50
    crypto_keys.push_back(std::make_pair(0x2ff385bd, 0xa2ed127f)); // 6.00.40
    crypto_keys.push_back(std::make_pair(0x2fbbb7cd, 0xa2c95e7f)); // 6.00.30
    crypto_keys.push_back(std::make_pair(0x2f63addd, 0xa2a5227f)); // 6.00.20
    crypto_keys.push_back(std::make_pair(0x2f2ba7ed, 0xa2817e7f)); // 6.00.10
    crypto_keys.push_back(std::make_pair(0x2f13a5fd, 0xa29d527f)); // 6.00.00
    crypto_keys.push_back(std::make_pair(0x2f6b076d, 0xa2a3be7f)); // 5.00.90
    crypto_keys.push_back(std::make_pair(0x2f53257d, 0xa2bf527f)); // 5.00.80
    crypto_keys.push_back(std::make_pair(0x2f1bc78d, 0xa29bfe7f)); // 5.00.70
    crypto_keys.push_back(std::make_pair(0x2fc3ed9d, 0xa2f4227f)); // 5.00.60
    crypto_keys.push_back(std::make_pair(0x2f8b97ad, 0xa2d0de7f)); // 5.00.50
    crypto_keys.push_back(std::make_pair(0x2e7385bd, 0xa22d127f)); // 5.00.40
    crypto_keys.push_back(std::make_pair(0x2e3bb7cd, 0xa2095e7f)); // 5.00.30
    crypto_keys.push_back(std::make_pair(0x2ee3addd, 0xa265227f)); // 5.00.20
    crypto_keys.push_back(std::make_pair(0x2eaba7ed, 0xa2417e7f)); // 5.00.10
    crypto_keys.push_back(std::make_pair(0x2e93a5fd, 0xa25d527f)); // 5.00.00
    crypto_keys.push_back(std::make_pair(0x2c7b574d, 0xa32d9e7f)); // 4.00.110
    crypto_keys.push_back(std::make_pair(0x2c236d5d, 0xa300a27f)); // 4.00.100
    crypto_keys.push_back(std::make_pair(0x2feb076d, 0xa2e3be7f)); // 4.00.90
    crypto_keys.push_back(std::make_pair(0x2fd3257d, 0xa2ff527f)); // 4.00.80
    crypto_keys.push_back(std::make_pair(0x2f9bc78d, 0xa2dbfe7f)); // 4.00.70
    crypto_keys.push_back(std::make_pair(0x2f43ed9d, 0xa2b4227f)); // 4.00.60
    crypto_keys.push_back(std::make_pair(0x2f0b97ad, 0xa290de7f)); // 4.00.50
    crypto_keys.push_back(std::make_pair(0x2ef385bd, 0xa26d127f)); // 4.00.40
    crypto_keys.push_back(std::make_pair(0x2ebbb7cd, 0xa2495e7f)); // 4.00.30
    crypto_keys.push_back(std::make_pair(0x2e63addd, 0xa225227f)); // 4.00.20
    crypto_keys.push_back(std::make_pair(0x2e2ba7ed, 0xa2017e7f)); // 4.00.10
    crypto_keys.push_back(std::make_pair(0x2e13a5fd, 0xa21d527f)); // 4.00.00
    crypto_keys.push_back(std::make_pair(0x2c53257d, 0xa33f527f)); // 3.00.80
    crypto_keys.push_back(std::make_pair(0x2c1bc78d, 0xa31bfe7f)); // 3.00.70
    crypto_keys.push_back(std::make_pair(0x2cc3ed9d, 0xa374227f)); // 3.00.60
    crypto_keys.push_back(std::make_pair(0x2c8b97ad, 0xa350de7f)); // 3.00.50
    crypto_keys.push_back(std::make_pair(0x2d7385bd, 0xa3ad127f)); // 3.00.40
    crypto_keys.push_back(std::make_pair(0x2d3bb7cd, 0xa3895e7f)); // 3.00.30
    crypto_keys.push_back(std::make_pair(0x2de3addd, 0xa3e5227f)); // 3.00.20
    crypto_keys.push_back(std::make_pair(0x2daba7ed, 0xa3c17e7f)); // 3.00.10
    crypto_keys.push_back(std::make_pair(0x2d93a5fd, 0xa3dd527f)); // 3.00.00
    crypto_keys.push_back(std::make_pair(0x2ceb076d, 0xa363be7f)); // 2.00.90
    crypto_keys.push_back(std::make_pair(0x2cd3257d, 0xa37f527f)); // 2.00.80
    crypto_keys.push_back(std::make_pair(0x2c9bc78d, 0xa35bfe7f)); // 2.00.70
    crypto_keys.push_back(std::make_pair(0x2c43ed9d, 0xa334227f)); // 2.00.60
    crypto_keys.push_back(std::make_pair(0x2c0b97ad, 0xa310de7f)); // 2.00.50
    crypto_keys.push_back(std::make_pair(0x2df385bd, 0xa3ed127f)); // 2.00.40
    /*
0200030 02dbbb7cd 0a3c95e7f enc_btfish // 2.00.30
0200020 02d63addd 0a3a5227f enc_btfish // 2.00.20
0200010 02d2ba7ed 0a3817e7f enc_btfish // 2.00.10
0200000 02d13a5fd 0a39d527f enc_btfish // = 2.00.00x
0200000 02d13a5fd 0a39d527f enc_bfish // 2.00.00
     */

    if (f.is_open())
    {
        bool read = true;
        bool server_type = false;
        while (read)
        {
            std::string line;
            while (std::getline(f, line)) {
                if (line.size() == 0)
                {
                    continue;
                }
                if (line.compare("[EOF]") == 0) {
                    TORUSSHELLECHO("Readed EOF block");
                    read = false;
                    break;
                } else if (line.compare("[SERVERS]") == 0) {
                    TORUSSHELLECHO("Readed SERVERS block");
                    server_type = true;
                } else if (line.find("[") != std::string::npos && line.find("]") != std::string::npos) {
                    //Other headers
                    server_type = false;
                } else {
                    std::vector<std::string> str(split(line, '='));
                    if (str.size() == 2) {
                        std::string key(str[0]);
                        std::string val(str[1]);
                        key = clean(key);
                        val = clean(val);
                        TORUSSHELLECHO("Found line with key = " << key << " and value = " << val);
                        if (server_type == true) { // [SERVERS]
                            std::vector<std::string> server(split(val, ','));
                            if (server.size() == 3) {
                                ServerInfo sinfo;
                                sinfo.name = std::string(server[0]);
                                sinfo.ip = std::string(server[1]);
                                sinfo.port = uword_t(atoi(server[2].c_str()));

                                if (key.compare("loginserver") == 0)
                                {
                                    login_server = sinfo;
                                    /*TORUSSHELLECHO(
                                            "Found " << key << " with name = " << sinfo.name << ", ip = "
                                                     << sinfo.ip << " and port = " << sinfo.port);*/
                                }
                                else if (key.compare("gameserver") == 0)
                                {
                                    add_game_server(sinfo);
                                    /*TORUSSHELLECHO(
                                            "Found " << key << " with name = " << sinfo.name << ", ip = "
                                                     << sinfo.ip << " and port = " << sinfo.port);*/
                                }
                            } else {
                                if (key.compare("gameserver_index") == 0)
                                {
                                    gameserver_index = atoi(val.c_str());
                                }
                            }
                        }
                        else // other [HEADERS]
                        {

                        }
                    }
                }
            }
        }
    }
}

void Config::_reset() {
    log_filename = "torusserver_log.txt";
    server_name = "TorusServer";
    server_desc = "TorusServer";
    torustable_file = "scripts/torustable.tscp";
    login_server.name = server_name;
    login_server.ip = "192.168.30.128";
    login_server.port = 2593;
    gameserver_index = 0;
    tick_duration = 250;
    tick_duration_overloaded = 225;
    tick_duration_idle = 30;
    file_path = "";

    obscene_strings.clear();
    fame_ranges.clear();
    fame_titles.clear();
    karma_ranges.clear();
    karma_titles.clear();
    noto_karma_ranges.clear();
    noto_fame_ranges.clear();
    noto_titles.clear();
    runes.clear();
    crypto_keys.clear();

    // Maps Section

    // Map Files
/*
    //NOTE: Add limit checks to do not exceed BASE_MAP_MAX value.
    MapFile map0new(0, 7168, 4096, 64, "map0.mul");// Map 0 (Felucca)    (New map size)
    map_files.push_back(map0new);
    // Example for old map style (older than ML). Don't enable both old and new, only one can be used.
    //MapFile map0old(0, 6144, 4096, 64, "map0.mul")  // Map 0 (Felucca)  (Old map size)
    //map_files.push_back(map0old);     
    MapFile map1(1, 7168, 4096, 64, "map1.mul");    // Map 1 (Trammel)
    map_files.push_back(map1);
    MapFile map2(2, 2304, 1600, 32, "map2.mul");    // Map 2 (Ilshenar)
    map_files.push_back(map2);
    MapFile map3(3, 2560, 2048, 32, "map3.mul");    // Map 3 (Malas)
    map_files.push_back(map3);
    MapFile map4(4, 1448, 1448, 8, "map4.mul");     // Map 4 (Tokuno Islands)
    map_files.push_back(map4);
    //MapFile map5(5, 1280, 4096, 32, "map5.mul");    // Map 5 (Ter Mur)
    //map_files.push_back(map5);

    //NOTE: Add limit checks to do not exceed VIRTUAL_MAP_MAX value.
    // Virtual (ingame) maps
    // IngameId, FileID
    maps.push_back(std::make_pair<t_ubyte, t_ubyte>(0, 0));
    maps.push_back(std::make_pair<t_ubyte, t_ubyte>(1, 1));
    maps.push_back(std::make_pair<t_ubyte, t_ubyte>(2, 2));
    maps.push_back(std::make_pair<t_ubyte, t_ubyte>(3, 3));
    maps.push_back(std::make_pair<t_ubyte, t_ubyte>(4, 4));
    maps.push_back(std::make_pair<t_ubyte, t_ubyte>(5, 5));
    //
    maps.push_back(std::make_pair<t_ubyte, t_ubyte>(10, 0));  // Adding map 10 as copy of map0.mul / Map0LegacyMUL.uop
    maps.push_back(std::make_pair<t_ubyte, t_ubyte>(11, 3));  // Adding map 11 as copy of map3.mul / Map3LegacyMUL.uop
*/
    // END Maps Section
    
}

void Config::add_game_server(ServerInfo &server) {
    //TODO: Verificate data.
    _game_servers.push_back(server);
}
void Config::add_game_server(std::string& name, std::string& ip, uword_t& port) {
    ServerInfo server(name, ip, port);
    add_game_server(server);
}
