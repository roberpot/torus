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

#include "errors.h"

NetworkError::NetworkError(const char * e) {
    _error = e;
}

NetworkError::NetworkError(const std::string e) {
    _error = e;
}

const char * NetworkError::what() {
    return _error.c_str();
}

std::ostream & NetworkError::operator<<(std::ostream & o) {
    o << "NetworkError(\"" << _error << "\")";
    return o;
}