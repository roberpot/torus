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

#ifndef __TORUS_CORE_ERRORS_H
#define __TORUS_CORE_ERRORS_H

#include <string>
#include <sstream>
#include "../library/types.h"

class NetworkError : public std::exception {
public:
    NetworkError(const t_byte * e);
    NetworkError(const std::string e);
    const t_byte * what();
    std::ostream & operator<<(std::ostream & o);
private:
    std::string _error;
};

class PostgresError : public std::exception {
public:
    PostgresError(const t_byte * e);
    PostgresError(const std::string e);
    const t_byte * what();
    std::ostream & operator<<(std::ostream & o);
private:
    std::string _error;
};

#define THROW_ERROR(CLASS, ERROR_MSG) { \
    std::stringstream s; \
    s << ERROR_MSG; \
    CLASS error(s.str().c_str()); \
    throw error; \
}

#endif //__TORUS_CORE_ERRORS_H
