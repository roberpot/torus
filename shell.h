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

#ifndef __TORUS_SHELL_H
#define __TORUS_SHELL_H

#include <sstream>
#include <fstream>
#include <threads/thread.h>
#include <library/types.h>

#define TORUSSHELLECHO(X) { \
    std::stringstream __torusshelecho__; \
    __torusshelecho__ << X; \
    torus_shell.print(__torusshelecho__.str()); \
}

#define TORUSSHELLECHOW(X) { \
    std::wstringstream __torusshelecho__; \
    __torusshelecho__ << X; \
    torus_shell.print(__torusshelecho__.str().c_str()); \
}

extern class TorusShell : public Thread {
public:
    TorusShell();
    void * run();
    void print(const std::string& str);
    void print(const wchar_t* s);
private:
    std::wofstream _log_file;
} torus_shell;

#endif //__TORUS_SHELL_H
