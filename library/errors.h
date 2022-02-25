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

#ifndef __TORUS_LIBRARY_ERRORS_H
#define __TORUS_LIBRARY_ERRORS_H

#include <library/types.h>

namespace ttl {
    class Exception {
    public:
        Exception();
        Exception(const t_byte* e);
        virtual const t_byte* what() const;
    protected:
        t_byte _msg[256];
    };

    class VectorError : public Exception {
    public:
        VectorError(const t_byte* e);
    };
    class StackError : public Exception {
    public:
        StackError(const t_byte* e);
    };

    class QueueError : public Exception {
    public:
        QueueError(const t_byte* e);
    };

    class ListError : public Exception {
    public:
        ListError(const t_byte* e);
    };

    class MapError : public Exception {
    public:
        MapError(const t_byte* e);
    };

    class LexicalError : public Exception {
    public:
        LexicalError(char c, int l);
    };

    class SyntaxError : public Exception {
    public:
        SyntaxError(int l);
    };

}


#endif //__TORUS_LIBRARY_ERRORS_H
