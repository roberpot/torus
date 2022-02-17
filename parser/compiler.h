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

#ifndef __TORUS_COMPILER_H
#define __TORUS_COMPILER_H

#include <string>
#include <library/queue.h>

extern class TorusCompiler {
public:
    void add_file(std::string f);
    void compile();
private:
    ttl::dynamicqueue<std::string> _files;
} toruscompiler;

#endif //__TORUS_COMPILER_H
