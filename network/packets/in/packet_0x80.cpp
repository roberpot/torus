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
#include <game/client.h>
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

const uword_t Packet_0x80::length()
{
    return 62;
}

void Packet_0x80::process(Socket* s)
{
    if (s->get_connection_state() != ConnectionState::CONNECTIONSTATE_LOGIN) // Double casting to prevent Warning #C26812 (VS)
    {
        return;
    }
    std::string account_name;
    std::string account_password;
    read_string(account_name, 30);
    read_string(account_password, 30);

    account_name = clean(account_name);
    account_password = clean(account_password);

    skip(1);    //Command: unused
    TORUSSHELLECHO("[LoginServer] Connection request to account " << account_name <<  ".");

    Account *acc = torusacc.get_account(account_name);

    if (acc == nullptr)
    {
        DEBUG_NOTICE("1");
        //LoginAck -> Invalid account
        s->get_client()->add_response_code(Packets::Out::Packet_0x82::ResponseCode::Invalid);
        return ;
    }
    else if (!acc->password_match(account_password))
    {
        DEBUG_NOTICE("2");
        s->get_client()->add_response_code(Packets::Out::Packet_0x82::ResponseCode::BadPass);
        //LoginAck -> Invalid pw
        return;
    }

    if (s == nullptr) { //Sometimes happens at clients' closure.
        DEBUG_NOTICE("3");
        s->get_client()->add_response_code(Packets::Out::Packet_0x82::ResponseCode::Invalid);
        return;
    }

    if (acc->connect(s))
    {
        DEBUG_NOTICE("Received valid account identification, proceeding to send server information.");
        ServerList* packet_server_list = new ServerList();
        packet_server_list->set_data(s);
        packet_server_list->send(s);
        s->set_connection_state(ConnectionState::CONNECTIONSTATE_SERVERLIST);
    }    
}

bool Packet_0x80::is_valid_account()
{
    return _is_valid_account;
}

}
}
