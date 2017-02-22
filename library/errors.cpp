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

#include <cstring>
#include "errors.h"

namespace ttl {
    Exception::Exception() {
        _msg = 0;
    }

    Exception::Exception(const t_byte * e) {
        _msg = new t_byte[strlen(e)];
        memcpy(_msg, e, sizeof(t_byte) * strlen(e));
    }

    Exception::~Exception() {
        if (_msg != 0) {
            delete _msg;
        }
    }

    const t_byte * Exception::what() const {
        if (_msg == 0) {
            return "No message supplied.";
        }
        return _msg;
    }

    VectorError::VectorError(const t_byte * e) : Exception::Exception(e) {
    }

    StackError::StackError(const t_byte * e) : Exception::Exception(e) {
    }

    QueueError::QueueError(const t_byte * e) : Exception::Exception(e) {
    }

    MapError::MapError(const t_byte * e) : Exception::Exception(e) {
    }
}
