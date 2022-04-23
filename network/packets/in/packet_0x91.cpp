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

using namespace ::Out;

const uword_t Packet_0x91::length()
{
    return 65;
}

void Packet_0x91::process(Socket* s)
{
  ADDTOCALLSTACK();
    //udword_t seed = read_udword();    //TODO: Seed matching check.
    skip(4);

    std::string account_name = read_string(CHARACTERS_STRING_LENGTH);
    std::string account_password = read_string(CHARACTERS_STRING_LENGTH);

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
        SupportedFeatures* packet_features = new SupportedFeatures();
        dword_t featureFlags = 1;
        packet_features->set_data(featureFlags, s->get_client());
        packet_features->send(s);

        CharList* packet_charlist = new CharList();
        packet_charlist->set_data(s->get_client());
        packet_charlist->send(s);
    }    
}

}
}