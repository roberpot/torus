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

#include <game/account.h>
#include <game/accounts_manager.h>
#include <library/string.h>
#include <network/packets/packetlist.h>
#include <network/socket.h>
#include <debug_support/info.h>
#include <shell.h>


namespace Packets
{
namespace In
{

const uword_t Packet_0x91::length()
{
    return 65;
}

void Packet_0x91::process(Socket* s)
{
    udword_t seed = read_udword();

    std::string account_name;
    std::string account_password;
    read_string(account_name, 30);
    read_string(account_password, 30);

    account_name = clean(account_name);
    account_password = clean(account_password);

    TORUSSHELLECHO("[GameServer] Connection request to account " << account_name <<  ".");

    Account* acc = torusacc.get_account(account_name);

    if (acc == nullptr)
    {
        //LoginAck -> Invalid account
        return;
    }
    else if (!acc->password_match(account_password))
    {
        //LoginAck -> Invalid pw
        return;
    }

    if (acc->connect(s))
    {
        DEBUG_NOTICE("Received valid account identification, proceeding to send client features and characters list.");
        Packets::Out::Packet_0xb9* packet = new Packets::Out::Packet_0xb9();
        dword_t featureFlags = 1;
        packet->set_data(featureFlags, s->get_client());
        packet->send(s);

        Packets::Out::Packet_0xa9* packetCharacterList = new Packets::Out::Packet_0xa9();
        packetCharacterList->set_data(s->get_client());
        packetCharacterList->send(s);
    }    
}

}
}