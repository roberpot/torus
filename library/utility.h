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

#ifndef __TORUS_UTILITY_H
#define __TORUS_UTILITY_H

#include "vector.h"
#include "stack.h"

namespace ttl {

    template <typename T>
    ttl::vector<T> stack_to_vector(ttl::fixedstack<T> s) {
        ttl::fixedstack<T> copy(s);
        ttl::vector<T> v;
        while (!copy.empty()) {
            v.push_back(copy.top());
            copy.pop();
        }
        return v;
    }

    template <typename T>
    ttl::vector<T> stack_to_vector(ttl::fixedgrowingstack<T> s) {
        ttl::fixedgrowingstack<T> copy(s);
        ttl::vector<T> v;
        while (!copy.empty()) {
            v.push_back(copy.top());
            copy.pop();
        }
        return v;
    }

    template <typename T>
    ttl::vector<T> stack_to_vector(ttl::dynamicstack<T> s) {
        ttl::dynamicstack<T> copy(s);
        ttl::vector<T> v;
        while (!copy.empty()) {
            v.push_back(copy.top());
            copy.pop();
        }
        return v;
    }

}

#endif //__TORUS_UTILITY_H
