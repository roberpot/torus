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


const udword_t Packet_0xa9::length() {
    ADDTOCALLSTACK();
    return 21;
}

void Packet_0xa9::set_data(Client* client)
{
    t_byte charCount = 1;
    write_byte(charCount);  //characters count
    for (t_byte i = 0; i < charCount; ++i)
    {
        //char[30] - name
        std::string nameStr;
        std::string tmpName = "XuN";
        nameStr.insert(0, 30 - tmpName.size(), 0);
        nameStr.insert(0, tmpName.c_str());
        write_string(nameStr, nameStr.size());
        //char[30] - password (empty?)
        std::string passwordStr(30, 0);
        write_string(passwordStr, passwordStr.size());
    }

    t_byte cityCount = 0;
    for (t_byte i = 0; i < cityCount; ++i)
    {
        /*
            char[32]	City Name
            char[32]	Building Name
            dword	City X
            dword	City Y
            dword	City Z
            dword	City Map
            dword	City Cliloc Description
            dword	0
        */
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
    write_word(0);
}

Packet_0xa9::~Packet_0xa9()
{
}