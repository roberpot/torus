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

#ifndef __TORUS_NETWORKMANAGER_H
#define __TORUS_NETWORKMANAGER_H

#include <vector>
#include <library/system_headers.h>



extern class NetworkManager : public Thread {
public:
    void * run();
    void halt();
    udword_t get_server_ip();
private:
    void _add_login_client(Socket* s);
    void _add_game_client(Socket* s);
    bool data_ready(fd_set &fd, const std::vector<Socket*>& socket_list);
    std::vector<Socket*> _login_sockets;
    std::vector<Socket*> _game_sockets;
    bool _run;
    class NetworkClientConnector : public Thread {
    public:
        NetworkClientConnector();
        void* run();
        void halt();
        udword_t get_server_ip();
    private:
        Socket* _loginserver;
        Socket* _gameserver;
        bool _run;
    } _clientconnector;
    Mutex _m;
} torusnet;

#endif //__TORUS_NETWORKMANAGER_H
