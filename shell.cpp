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

#include <ctime>
#include <iostream>
#include <string>

#include "library/string.h"
#include "core/torus.h"
#include "shell.h"

std::string _get_curr_datetime() {
    std::string r;
    char buffer[100];
    std::time_t t = std::time(NULL);
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    r = buffer;
    return r;
}

TorusShell::TorusShell() {
}

void * TorusShell::run() {
    bool continue_proc = true;
    char c;
    char buffer[256];
    char * stripped_buffer;
    unsigned int counter = 0;
    //DBG_MSG_INF("Starting User Interface Thread.");
    do {
        c = (char)getchar();
        if (c != 10) {
            buffer[counter] = c;
            if (counter < 255) {
                counter++;
            }
        } else {
            buffer[counter] = 0;
            stripped_buffer = buffer;
            if (counter > 0) {
                STRIP(stripped_buffer);
            }
            if (strlen(stripped_buffer) == 1 && stripped_buffer[0] == 'x') {
                continue_proc = false;
                torus.stop();
            }
//            if (torusshell.execute_order(stripped_buffer)) {
//                continue_proc = false;
//            } else {
//                counter = 0;
//            }
            counter = 0;
        }
    } while (continue_proc);
    //DBG_MSG_INF("User Interface Thread finished.");
    return NULL;
}

void TorusShell::print(const char * s) {
    std::string t = _get_curr_datetime();
    std::cout << "[" << t << "] " << s << std::endl;
    //log_file << "[" << t << "] " << c << std::endl;
}