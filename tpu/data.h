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

#ifndef __TORUS_TPU_DATA_H
#define __TORUS_TPU_DATA_H

#include <library/types.h>

namespace tpu {
    enum class DataType {
        UNKNOWN,
        NO_TYPE,
        INTEGER,
        STRING
    };

    class Data {
    public:
        Data();
        Data(bool c, qword_t d);
        Data(bool c, const std::string& d);
        Data(bool c, const Data& o);
        ~Data();
        bool is_const() const;
        DataType type() const;
        qword_t get_integer();
        std::string get_string();
    private:
        bool _const;
        DataType _type;
        union {
            qword_t _integer;
            std::string _string;
        };
    };
}

#endif //__TORUS_TPU_DATA_H
