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
    while (_run) {
        _m.lock();
        size_t socketsCount = _sockets.size();
        Socket * clientSocket;
        Packet * newPacket;
        for (unsigned int socketId = 0; socketId < socketsCount; socketId++) {
            clientSocket = _sockets[socketId];
            while ((clientSocket->is_closing() == false) && clientSocket->data_ready()) {
                newPacket = clientSocket->read_packet();
                delete newPacket;
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


NetworkManager::NetworkClientConnector::NetworkClientConnector() {
    _s = 0;
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
    _s = new Socket;
    _s->bind(toruscfg.net_addr, toruscfg.net_port);

    while(_run) {
        if (_s->client_pending()) {
            Socket * s = _s->get_socket();
            TORUSSHELLECHO("Client connected: IP: " << s->get_ip());
            torusnet._add_client(s);
        }
        if (_s->is_closing())
        {
            _s->shutdown();
        }
        torus_thread_sleep(50);
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
