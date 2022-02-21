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


//const udword_t Packet_0xa9::length() {
//    ADDTOCALLSTACK();
//    return 21;
//}

void PacketOut_0xa9::set_data(Client* client)
{
    UNREFERENCED_PARAMETER(client); // TODO: Gather packet's data from client.


    //std::string paste("a904560558754e0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000900596577000000000000000000000000000000000000000000000000000000000054686520456d7061746820416262657900000000000000000000000000000000000002790000035a0000000000000000001067000000000014d696e6f63000000000000000000000000000000000000000000000000000000546865204261726e61636c650000000000000000000000000000000000000000000009ac0000019d0000000f00000000001067810000000002427267461696e00000000000000000000000000000000000000000000000000537765657420447265616d7320496e6e00000000000000000000000000000000000005d80000065c0000000a000000000010678200000000034d6f6f6e676c6f7700000000000000000000000000000000000000000000000546865205363686f6c61727320496e6e00000000000000000000000000000000000011380000049000000000000000000010678300000000045472696e73696300000000000000000000000000000000000000000000000005468652054726176656c6c6572277320496e6e000000000000000000000000000000073500000ab900000000000000000010678400000000054d6167696e6369610000000000000000000000000000000000000000000000054686520477265617420486f726e732054617665726e0000000000000000000000000e96000008ae000000140000000000106785000");

    //write_from_paste(paste);
    //return;

    const int acc_name_pw_len = 30;
    const int city_strings_len = 32; //Changed for newer clients from 30 to 32.

    t_byte charCount = 1;
    write_byte(charCount);  //characters count
    for (t_byte i = 0; i < charCount; ++i)
    {
        std::string nameStr = "XuN";
        size_t j = nameStr.size();
        for (; j < acc_name_pw_len; ++j)
        {
            nameStr.push_back('\0');
        }
        //*this << nameStr;
        std::string passwordStr = "";
        j = passwordStr.size();
        for (; j < acc_name_pw_len; ++j)
        {
            passwordStr.push_back('\0');
        }
        //*this << passwordStr;
    }

    std::vector<std::string> cities;
    cities.push_back("britain");
    cities.push_back("cove");
    t_byte cityCount = t_byte(cities.size()); // FIXME: Real cities configuration
    for (t_byte i = 0; i < cityCount; ++i)
    {
        std::string city = cities[i];
        size_t j = city.size();
        for (; j < city_strings_len; ++j)
        {
            city.push_back('\0');
        }

        std::string cityStr(city);
        write_string(cityStr); // City Name
        write_string(cityStr); // Building Name
        write_dword(i);         // X
        write_dword(i + 1);     // Y
        write_dword(i + 2);     // Z
        write_dword(0);         // Map
        write_dword(0);         // Cliloc
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
    write_dword(0x100);
    write_word(0);
}

PacketOut_0xa9::PacketOut_0xa9() : PacketOut(0xa9)
{
}

PacketOut_0xa9::~PacketOut_0xa9()
{
}