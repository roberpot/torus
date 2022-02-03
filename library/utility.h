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

#include <library/queue.h>
#include <library/map.h>
#include <library/stack.h>
#include <library/vector.h>

namespace ttl {

    staticstack<udword_t> __a;
    staticgrowingstack<udword_t> __b;
    dynamicstack<udword_t> __c;
    tsfixedstack<udword_t> __d;
    tsfixedgrowingstack<udword_t> __e;
    tsdynamicstack<udword_t> __f;
    vector<udword_t> __g;
    tsvector<udword_t> __h;
    staticqueue<udword_t> __i;
    staticgrowingqueue<udword_t> __j;
    dynamicqueue<udword_t> __k;
    tsfixedqueue<udword_t> __l;
    tsfixedgrowingqueue<udword_t> __m;
    tsdynamicqueue<udword_t> __n;
    staticgrowingmap<udword_t, udword_t> __o;
    dynamicmap<udword_t, udword_t> __q;

    template <typename T>
    ttl::vector<T> stack_to_vector(ttl::staticstack<T> s) {
        ttl::staticstack<T> copy(s);
        ttl::vector<T> v;
        while (!copy.empty()) {
            v.push_back(copy.top());
            copy.pop();
        }
        return v;
    }

    template <typename T>
    ttl::vector<T> stack_to_vector(ttl::staticgrowingstack<T> s) {
        ttl::staticgrowingstack<T> copy(s);
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
