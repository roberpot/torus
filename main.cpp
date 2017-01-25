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

#include <iostream>

#include "core/torus.h"
#include "shell.h"
#include "debug/debug.h"
#include "core/slave.h"
#include "network/networkmanager.h"

TorusShell torus_shell;
Torus torus;

int main() {
    torus_shell.start();
    TORUSSHELLECHO("Initializing Torus...");
    torusnet.start();
    TORUSSHELLECHO("Initializing Torus... OK.");
    SlaveThread s;
    s.start();
    while (torus.keep_running()) {

    }
    s.join();
    torusnet.halt();
    torusnet.join();
    TORUSSHELLECHO("Shutting down Torus...");
    TORUSSHELLECHO("Shutting down Torus... OK.");
    torus_shell.join();
    return 0;
}