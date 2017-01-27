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

#include "networkmanager.h"
#include "../core/config.h"
#include "../shell.h"
#include "../threads/utils.h"
#include "../debug/info.h"
#include "../debug/callstack.h"
#include "../core/errors.h"

class NetworkManager torusnet;

void * NetworkManager::run() {
    ADDTOCALLSTACK();
    EXC_TRY("");
    _run = true;
    _clientconnector.start();
    while (_run) {
        _m.lock();
        size_t l = _sockets.size();
        Socket * s;
        Packet * p;
        UNREFERENCED_PARAMETER(p);
        for (unsigned int i = 0; i < l; i++) {
            s = _sockets[i];
            while (s->data_ready()) {
                p = s->read_packet();
            }
        }
        _m.unlock();
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


NetworkManager::NetworkClientConector::NetworkClientConector() {
    _s = 0;
    _run = false;
}

void * NetworkManager::NetworkClientConector::run() {
    ADDTOCALLSTACK();
    EXC_TRY("");
    _run = true;
#ifdef _WINDOWS
    WSADATA wsadata;
    t_dword status;
    status = WSAStartup(MAKEWORD(2,2), &wsadata);
    if (status != 0) {
        THROW_ERROR(NetworkError, "WSAStartup failed with error: " << status);
    }
#endif //_WINDOWS
    _s = new Socket;
    _s->bind(toruscfg.net_addr, toruscfg.net_port);

    while(_run) {
        if (_s->client_pending()) {
            Socket * s = _s->get_client();
            TORUSSHELLECHO("Client connected: IP: " << s->get_ip());
            torusnet._add_client(s);
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

void NetworkManager::NetworkClientConector::halt() {
    _run = false;
}
