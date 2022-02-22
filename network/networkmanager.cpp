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
        size_t socketsCount = _sockets.size();
        Socket * clientSocket;
        FD_ZERO(&readSet);
        if (data_ready(readSet))
        {
            for (unsigned int socketId = 0; socketId < socketsCount; ++socketId) {
                clientSocket = _sockets[socketId];
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
                TORUSSHELLECHO("data ready for socket, reading it from" << clientSocket->get_ip_str());
                if (clientSocket->receive() == false)
                {
                    continue;
                }
                FD_CLR(clientSocket->get_socket(), &readSet);
            }
            _sockets.clear();
            _sockets = v_tmp_sockets;
            v_tmp_sockets.clear();
        }
        for (unsigned int socketId = 0; socketId < socketsCount; ++socketId)
        {
            clientSocket = _sockets[socketId];

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

void NetworkManager::_add_client(Socket * s) {
    _m.lock();
    _sockets.push_back(s);
    _m.unlock();
}

bool NetworkManager::data_ready(fd_set& fd)
{
    int count = 0;
    for (size_t i = 0; i < _sockets.size(); ++i)
    {
        int s = _sockets[i]->get_socket();
        FD_SET(s, &fd);
#ifdef __WINDOWS
        ++count;
#endif
#ifdef __linux__
        if (count < s)
        {
            s = count;
        }
#endif
    }
    timeval timeout; // time to wait for data.
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;
    int res = select(count + 1, &fd, nullptr, nullptr, &timeout);
    return  res > 0;
}

udword_t NetworkManager::get_server_ip() {
    return _clientconnector.get_server_ip();
}

NetworkManager::NetworkClientConnector::NetworkClientConnector() {
    _loginserver = 0;
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
            Socket * s = _loginserver->create_socket(addr);
            TORUSSHELLECHO("Client connected: IP: " << s->get_ip_str());
            torusnet._add_client(s);
        }
        /*if (_s->is_closing())
        {
            TORUSSHELLECHO("Closing socket: IP" << _s->get_ip());
            _s->shutdown();
        }*/
        torus_thread_sleep(1);
    }

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
}

udword_t NetworkManager::NetworkClientConnector::get_server_ip() {
    return _gameserver->get_ip();
}
