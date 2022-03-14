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

#ifndef __TORUS_DEFNAME_H
#define __TORUS_DEFNAME_H

#include <string>

#include <library/types.h>

class Defname {
public:
};

class DefnameNamespace {
public:
    DefnameNamespace(const std::string& ns, const std::string& file, uqword_t line);
private:
    std::string _namespace;
    std::string _file;
    uqword_t _line;
};

#endif //__TORUS_DEFNAME_H
