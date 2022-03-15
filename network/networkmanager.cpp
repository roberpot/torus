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

#include <network/networkmanager.h>
#include <core/config.h>
#include <core/errors.h>
#include <shell.h>
#include <threads/utils.h>
#include <debug_support/callstack.h>
#include <debug_support/info.h>

class NetworkManager torusnet;

void * NetworkManager::run() {
    ADDTOCALLSTACK();
    EXC_TRY("");
    _run = true;
    _clientconnector.start();
    std::vector<Socket *> v_tmp_sockets;
    fd_set readSet;
    while (_run) {
        _m.lock();
        size_t socketsCount;;
        Socket * clientSocket;
        socketsCount = _game_sockets.size();
        FD_ZERO(&readSet);

        if (data_ready(readSet, _game_sockets))
        {
            for (unsigned int socketId = 0; socketId < socketsCount; ++socketId) {
                clientSocket = _game_sockets[socketId];
                v_tmp_sockets.push_back(clientSocket);
                if (clientSocket->is_read_closed())
                {
                    continue;
                }
                if (!FD_ISSET(clientSocket->get_socket(), &readSet))
                {
                    clientSocket->clean_incoming_packets();
                    continue;
                }
                //TORUSSHELLECHO("[GameServer]: Data ready for socket " << clientSocket << ", from IP " << clientSocket->get_ip_str());
                if (clientSocket->receive() == false)
                {
                    continue;
                }
                FD_CLR(clientSocket->get_socket(), &readSet);
            }
            _game_sockets.clear();
            _game_sockets = v_tmp_sockets;
            socketsCount = _game_sockets.size();
            v_tmp_sockets.clear();
        }
        for (unsigned int socketId = 0; socketId < socketsCount; ++socketId)
        {
            clientSocket = _game_sockets[socketId];

            if (!clientSocket->is_read_closed())
            {
                clientSocket->read_queued_packets();
            }
            if (!clientSocket->is_write_closed())
            {
                clientSocket->send_queued_packets();
            }
        }

        FD_ZERO(&readSet);
        socketsCount = _login_sockets.size();
        if (data_ready(readSet, _login_sockets))
        {
            for (unsigned int socketId = 0; socketId < socketsCount; ++socketId) {
                clientSocket = _login_sockets[socketId];
                if (clientSocket->is_read_closed() || clientSocket->is_write_closed())
                {
                    //clientSocket->shutdown();
                    continue;
                }
                v_tmp_sockets.push_back(clientSocket);
                if (!FD_ISSET(clientSocket->get_socket(), &readSet))
                {
                    clientSocket->clean_incoming_packets();
                    continue;
                }
                //TORUSSHELLECHO("[LoginServer]: Data ready for socket " << clientSocket << ", from IP " << clientSocket->get_ip_str());
                if (clientSocket->receive() == false)
                {
                    continue;
                }
                FD_CLR(clientSocket->get_socket(), &readSet);
            }
            _login_sockets.clear();
            _login_sockets = v_tmp_sockets;
            socketsCount = _login_sockets.size();
            v_tmp_sockets.clear();
        }
        for (unsigned int socketId = 0; socketId < socketsCount; ++socketId)
        {
            clientSocket = _login_sockets[socketId];

            if (!clientSocket->is_read_closed())
            {
                clientSocket->read_queued_packets();
            }
            if (!clientSocket->is_write_closed())
            {
                clientSocket->send_queued_packets();
            }
        }
        _m.unlock();
        torus_thread_sleep(50);
    }
    _clientconnector.halt();
    _clientconnector.join();
    EXC_CATCH;
    EXC_DEBUG_START;
    EXC_DEBUG_END;
    return 0;
}

void NetworkManager::halt() {
    _run = false;
}

udword_t NetworkManager::get_server_ip()
{
    return _clientconnector.get_server_ip();
}

void NetworkManager::_add_game_client(Socket * s) {
    _m.lock();
    _game_sockets.push_back(s);
    _m.unlock();
}

void NetworkManager::_add_login_client(Socket* s) {
    _m.lock();
    _login_sockets.push_back(s);
    _m.unlock();
}

bool NetworkManager::data_ready(fd_set& fd, const std::vector<Socket*>& socket_list)
{
    int count = 0;
    for (size_t i = 0; i < socket_list.size(); ++i)
    {
        int s = int(socket_list[i]->get_socket());
        FD_SET(s, &fd);
#ifdef __WINDOWS
        ++count;
#endif
#ifdef __linux__
        if (s > count)
        {
            count = s;
        }
#endif
    }
    timeval timeout; // time to wait for data.
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;
    int res = select(count + 1, &fd, nullptr, nullptr, &timeout);
    return  res > 0;
}

NetworkManager::NetworkClientConnector::NetworkClientConnector() {
    _loginserver = 0;
    _gameserver = 0;
    _run = false;
}

void * NetworkManager::NetworkClientConnector::run() {
    ADDTOCALLSTACK();
    EXC_TRY("");
    _run = true;
#ifdef _WINDOWS
    WSADATA wsadata;
    dword_t status;
    status = WSAStartup(MAKEWORD(2,2), &wsadata);
    if (status != 0) {
        THROW_ERROR(NetworkError, "WSAStartup failed with error: " << status);
    }
#endif //_WINDOWS
    _loginserver = new Socket(ConnectionType::CONNECTIONTYPE_LOGINSERVER);
    _loginserver->bind(toruscfg.login_server.ip.c_str(), toruscfg.login_server.port);

    std::string gameip(toruscfg._game_servers[toruscfg.gameserver_index].ip.c_str());

    _gameserver = new Socket(ConnectionType::CONNECTIONTYPE_LOGINSERVER);
    _gameserver->bind(gameip.c_str(),
                      toruscfg._game_servers[toruscfg.gameserver_index].port);
    while(_run) {
        sockaddr_in addr;
        if (_loginserver->client_pending(addr)) {
            Socket * s = _loginserver->create_socket(addr, ConnectionType::CONNECTIONTYPE_LOGINSERVER);
            TORUSSHELLECHO("Client (" << s << ") connected to LoginServer: IP: " << s->get_ip_str());
            torusnet._add_login_client(s);
        }
        if (_gameserver->client_pending(addr)) {
            Socket* s = _gameserver->create_socket(addr, ConnectionType::CONNECTIONTYPE_GAMESERVER);
            TORUSSHELLECHO("Client (" << s << ") connected to GameServer: IP: " << s->get_ip_str());
            torusnet._add_game_client(s);
        }
        torus_thread_sleep(1);
    }
    _loginserver->shutdown();
    _gameserver->shutdown();

#ifdef _WINDOWS
    WSACleanup();
#endif //_WINDOWS
    EXC_CATCH;
    EXC_DEBUG_START;
    EXC_DEBUG_END;
    return 0;
}

void NetworkManager::NetworkClientConnector::halt() {
    _run = false;
    _loginserver->shutdown();
    _gameserver->shutdown();
}

udword_t NetworkManager::NetworkClientConnector::get_server_ip() {
    return _gameserver->get_ip();
}
