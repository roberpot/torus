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
#include <network/socket.h>
#include <threads/thread.h>
#include <threads/mutex.h>


extern class NetworkManager : public Thread {
public:
    void * run();
    void halt();
private:
    void _add_client(Socket * s);
    std::vector<Socket *> _sockets;
    bool _run;
    class NetworkClientConector : public Thread {
    public:
        NetworkClientConector();
        void * run();
        void halt();
    private:
        Socket * _s;
        bool _run;
    } _clientconnector;
    Mutex _m;
} torusnet;

#endif //__TORUS_NETWORKMANAGER_H
