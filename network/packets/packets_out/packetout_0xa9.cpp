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

#include <network/packets/packetlist.h>
#include <network/socket.h>
#include <debug_support/info.h>
#include <library/types.h>


//const udword_t Packet_0xa9::length() {
//    ADDTOCALLSTACK();
//    return 21;
//}

struct MapPoint {
    udword_t x;
    udword_t y;
    udword_t z;
    udword_t m;
    MapPoint(udword_t px, udword_t py, udword_t pz, udword_t pm) : x(px), y(py), z(pz), m(pm) {};
};

void PacketOut_0xa9::set_data(Client* client)
{
    UNREFERENCED_PARAMETER(client); // TODO: Gather packet's data from client.


    const int acc_name_pw_len = 30;
    const int city_strings_len = 32; //Changed for newer clients from 30 to 32.


    std::vector<std::string> characters{"XuN", "", "", "", ""};
    t_byte charCount = t_byte(maximum(int(characters.size()), 5)); // Must send always 5, client bug.
    write_byte(charCount);  //characters count
    for (t_byte i = 0; i < charCount; ++i)
    {
        std::string nameStr = characters[i];
        size_t j = nameStr.size();
        for (; j < acc_name_pw_len; ++j)
        {
            nameStr.push_back('\000');
        }
        write_string(nameStr); // Player Name
        std::string passwordStr = "";
        j = passwordStr.size();
        for (; j < acc_name_pw_len; ++j)
        {
            passwordStr.push_back('\000');
        }
        write_string(passwordStr);
    }


    //Starting Points: TODO: Read correctly.
    std::vector<std::string> cities;
    cities.push_back("Yew");

    std::vector<std::string> buildings;
    buildings.push_back("The Empath Abbey");

    std::vector<MapPoint> points;
    points.push_back(MapPoint(633, 858, 0, 0));

    std::vector<dword_t> clilocs;
    clilocs.push_back(1075072);


    t_byte cityCount = t_byte(cities.size()); // FIXME: Real cities configuration
    write_byte(cityCount);
    for (t_byte i = 0; i < cityCount; ++i)
    {
        std::string city = cities[i];
        size_t j = city.size();
        for (; j < city_strings_len; ++j)
        {
            city.push_back('\000');
        }
        std::string building = buildings[i];
        for (j = building.size(); j < city_strings_len; ++j)
        {
            building.push_back('\000');
        }
        MapPoint point = points[i];
        write_byte(i);
        write_string(city);     // City Name
        write_string(building); // Building Name
        write_dword(point.x);   // X
        write_dword(point.y);   // Y
        write_dword(point.z);   // Z
        write_dword(point.m);   // Map
        write_dword(clilocs[i]);// Cliloc
        write_dword(0);         // Unk
    }

    //character creationg flags
    /*Flags
        (0x01 = unknown flag1;
        0x02 = overwrite configuration button;
        0x04 = limit 1 character per account;
        0x08 = enable context menus;
        0x10 = limit character slots;
        0x20 = paladin and necromancer classes, tooltips;
        0x40 = 6th character slot;
        0x80 = samurai and ninja classes;
        0x100 = elven race;
        0x200 = unknown flag2;
        0x400 = send UO3D client type (client will send 0xE1 packet);
        0x800 = unknown flag3;
        0x1000 = 7th character slot;
        0x2000 = unknown flag4;
        0x4000 = new movement system;
        0x8000 = unlock new felucca areas)
    */
    write_dword(1960);
    //write_word(0); // Only for enhanced client.
}

PacketOut_0xa9::PacketOut_0xa9() : PacketOut(0xa9, true)
{
}

PacketOut_0xa9::~PacketOut_0xa9()
{
}