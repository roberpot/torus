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

#ifndef __TORUS_MAP_H
#define __TORUS_MAP_H

#include <cstring>
#include "types.h"
#include "errors.h"
#include "../threads/mutex.h"

#ifdef _MSC_VER
    #pragma warning(disable: 4521 4522)
#endif

#define _TTL_MAP_DEFAULT_SIZE 10

namespace ttl {

    template<typename K, typename V>
    class staticmap {
    public:
        class CellPosition {
        public:
            staticmap * map;
            bool exists;
            K key;
            t_udword position;
            virtual V operator=(V x) {
                if (!exists) {
                    map->_reserve(map->_size + 1);
                    memmove(&map->_map[position + 1], &map->_map[position], sizeof(Cell) * (map->_size - position));
                    map->_size++;
                    map->_map[position].key = key;
                }
                map->_map[position].value = x;
                return x;
            }
            virtual operator V () {
                if (!exists) {
                    throw MapError("Key not found");
                }
                return map->_map[position].value;
            }
        };

        staticmap(t_udword size=_TTL_MAP_DEFAULT_SIZE) {
            _capacity = size;
            _size = 0;
            _map = new Cell[_capacity];
        }
        staticmap(staticmap & o) {
            _capacity = o._capacity;
            _size = o._size;
            _map = new Cell[_capacity];
            memcpy(_map, o._map, sizeof(Cell) * _capacity);
        }
        virtual ~staticmap() {
            delete[] _map;
        }
        staticmap & operator=(staticmap & o) {
            delete _map;
            _capacity = o._capacity;
            _size = o._size;
            _map = new Cell[_capacity];
            memcpy(_map, o._map, sizeof(Cell) * _capacity);
            return *this;
        }
        virtual bool empty() { return _size == 0; }
        virtual t_udword size() { return _size; }
        virtual t_udword max_size() { return TDWORD_MAX; }
        CellPosition operator[](K key) {
            CellPosition p;
            t_udword init = 0, end = _size - 1, pivot = 0;
            K pivot_key;
            bool stop = false;
            while ( init > end && !stop) {
                pivot = (end - init) / 2 + init;
                pivot_key = _map[pivot].key;
                if (pivot_key == key) {
                    init = pivot;
                    stop = true;
                } else if (key < pivot_key) {
                    end = pivot - 1;
                } else {
                    init = pivot + 1;
                }
            }
            p.map = this;
            p.exists = pivot_key == key;
            p.key = key;
            p.position = pivot;
            return p;
        }
        virtual void erase(K key) {
            CellPosition p = (*this)[key];
            if (p.exists) {
                memmove(&_map[p.position], &_map[p.position + 1], sizeof(Cell) * (_size - p.position));
                _size--;
            }
        }
        void swap(staticmap & o) {
            Cell * map = _map;
            t_udword size = _size;
            t_udword capacity = _capacity;
            _map = o._map;
            _size = o._size;
            _capacity = o._capacity;
            o._map = map;
            o._size = size;
            o._capacity = capacity;
        }
        virtual void clear() {
            _size = 0;
        }
        virtual t_udword count(K key) {
            CellPosition p = (*this)[key];
            if (p.exists) { return 1; }
            return 0;
        }
    protected:
        class Cell {
        public:
            K key;
            V value;
        };
        virtual void _reserve(t_udword n) {
            if (_capacity < n) {
                _resize(n + _TTL_MAP_DEFAULT_SIZE);
            }
        }
        virtual void _resize(t_udword s) {
            Cell * n = new Cell[s];
            t_udword nsize = _size;
            if (s < _size) nsize = s;
            memcpy(n, _map, sizeof(Cell) * nsize);
            _size = nsize;
            _capacity = s;
            delete _map;
            _map = n;
        }
        Cell * _map;
        t_udword _capacity;
        t_udword _size;
    };

    template<typename K, typename V>
    class dynamicmap {
    protected:
        class Node {
        public:
            K key;
            V value;
            Node * l, * r, *p;
            Node() : l(NULL), r(NULL), p(NULL) {}
            Node(Node & o) {
                key = o.key;
                value = o.value;
                l = NULL;
                r = NULL;
                p = o.p;
                if (o.l) { l = new Node(*(o.l)); }
                if (o.r) { r = new Node(*(o.r)); }
            }
            ~Node() {
                if (l) { delete l; }
                if (r) { delete r; }
            }
            t_udword child_count() {
                t_udword count = 0;
                if (l != NULL) count++;
                if (r != NULL) count++;
                return count;
            }
            void replace_by_child() {
                if (r != NULL and l != NULL) {
                    throw MapError("Tree inconsistency.");
                }
                Node * replacement;
                if (l != NULL) {
                    replacement = l;
                    l = NULL;
                } else {
                    replacement = r;
                    r = NULL;
                }
                replacement->p = p;
                if (p != NULL) {
                    if (p->l == this) {
                        p->l = replacement;
                    } else if (p->r == this) {
                        p->r = replacement;
                    } else {
                        throw MapError("Tree inconsistency.");
                    }
                }
            }
            void remove_child(Node * n) {
                if (l == n) {
                    l = NULL;
                } else if (r == n) {
                    r = NULL;
                } else {
                    throw MapError("Tree inconsistency.");
                }
                n->p = NULL;
            }
        };
        Node * _root;
        t_udword _size;
    public:
        class CellPosition {
        public:
            dynamicmap * map;
            Node * n;
            bool exists;
            K key;
            virtual V operator=(V x) {
                if (!exists) {
                    Node * n2 = new Node();
                    n2->key = key;
                    n2->value = x;
                    if (key < n->key) { n->l = n2; }
                    if (key > n->key) { n->r = n2; }
                    n2->p = n;
                    n = n2;
                    map->_size++;
                }
                n->value = x;
                return x;
            }
            virtual operator V () {
                if (!exists) {
                    throw MapError("Key not found");
                }
                return n->value;
            }
        };

        dynamicmap() {
            _root = NULL;
            _size = 0;
        }
        dynamicmap(dynamicmap & o) {
            _root = new Node(*(o._root));
            _size = o._size;
        }
        virtual ~dynamicmap() {
            delete _root;
        }
        dynamicmap & operator=(dynamicmap & o) {
            delete _root;
            _root = new Node(*(o._root));
            _size = o._size;
        }
        virtual bool empty() { return _size == 0; }
        virtual t_udword size() { return _size; }
        CellPosition operator[](K key) {
            CellPosition p;
            Node * n = _root, * n_ant = NULL;
            while(n != NULL && n->key != key) {
                n_ant = n;
                if (n->key < key) { n = n->r; }
                else { n = n->l; }
            }
            p.map = this;
            p.exists = n != NULL;
            p.key = key;
            if (p.exists) { p.n = n; }
            else { p.n = n_ant; }
            return p;
        }
        virtual void erase(K key) {
            CellPosition p = (*this)[key];
            if (!p.exists) {
                throw MapError("Key not found.");
            }
            Node * n = p.n;
            // Case leaf:
            t_udword childs = n->child_count();
            if (childs == 0) {
                n->p->remove_child(n);
                delete n;
            // Case one child.
            } else if (childs == 1) {
                n->replace_by_child();
                delete n;
            // Case two childs.
            } else {
                // Search for a minimum.
                Node * min = n->r;
                while (min->l != NULL) min = min->l;
                if (min->child_count() == 1) {
                    min->replace_by_child();
                } else {
                    min->p->remove_child(min);
                }
                n->key = min->key;
                n->value = min->value;
                delete min;
            }
            _size--;
        }
        void swap(dynamicmap & o) {
            Node * tmproot = _root;
            t_udword tmpsize = _size;
            _root = o._root;
            _size = o._size;
            o._root = tmproot;
            o._size = tmpsize;
        }
        virtual void clear() {
            delete _root;
            _size = 0;
        }
        virtual t_udword count(K key) {
            CellPosition p = (*this)[key];
            if (p.exists) { return 1; }
            return 0;
        }
    };

    template<typename K, typename V>
    class tsstaticmap : public staticmap<K, V> {
    public:
        class tsCellPosition : public staticmap<K, V>::CellPosition {
        public:
            tsstaticmap * map;
            bool exists;
            K key;
            t_udword position;
            V operator=(V x) {
                tsCellPosition::operator=(x);
                map->_mutex.unlock();
                return x;
            }
            operator V () {
                try {
                    V x = staticmap<K,V>::CellPosition::operator V();
                    map->_mutex.unlock();
                    return x;
                }
                catch (MapError & e) {
                    map->_mutex.unlock();
                    throw;
                }
            }
        };

        tsstaticmap(t_udword size=_TTL_MAP_DEFAULT_SIZE) {
            staticmap<K,V>::_capacity = size;
            staticmap<K,V>::_size = 0;
            staticmap<K,V>::_map = new typename staticmap<K,V>::Cell[staticmap<K,V>::_capacity];
        }
        tsstaticmap(tsstaticmap & o) {
            o._mutex.lock();
            staticmap<K,V>::_capacity = o._capacity;
            staticmap<K,V>::_size = o._size;
            staticmap<K,V>::_map = new typename staticmap<K,V>::Cell[staticmap<K,V>::_capacity];
            memcpy(staticmap<K,V>::_map, o._map, sizeof(typename staticmap<K,V>::Cell) * staticmap<K,V>::_capacity);
            o._mutex.unlock();
        }
        virtual ~tsstaticmap() {
            delete staticmap<K,V>::_map;
        }
        tsstaticmap & operator=(tsstaticmap & o) {
            _mutex.lock();
            o._mutex.lock();
            delete staticmap<K,V>::_map;
            staticmap<K,V>::_capacity = o._capacity;
            staticmap<K,V>::_size = o._size;
            staticmap<K,V>::_map = new typename staticmap<K,V>::Cell[staticmap<K,V>::_capacity];
            memcpy(staticmap<K,V>::_map, o._map, sizeof(typename staticmap<K,V>::Cell) * staticmap<K,V>::_capacity);
            o._mutex.unlock();
            _mutex.unlock();
            return *this;
        }
        bool empty() {
            _mutex.lock();
            bool b = staticmap<K,V>::_size == 0;
            _mutex.unlock();
            return b;
        }
        t_udword size() {
            _mutex.lock();
            t_udword s = staticmap<K,V>::_size;
            _mutex.unlock();
            return s;
        }
        t_udword max_size() { return TDWORD_MAX; }
        tsCellPosition operator[](K key) {
            _mutex.lock();
            tsCellPosition p;
            t_udword init = 0, end = staticmap<K,V>::_size - 1, pivot = 0;
            K pivot_key;
            bool stop = false;
            while ( init > end && !stop) {
                pivot = (end - init) / 2 + init;
                pivot_key = staticmap<K,V>::_map[pivot].key;
                if (pivot_key == key) {
                    init = pivot;
                    stop = true;
                } else if (key < pivot_key) {
                    end = pivot - 1;
                } else {
                    init = pivot + 1;
                }
            }
            p.map = this;
            p.exists = pivot_key == key;
            p.key = key;
            p.position = pivot;
            return p;
        }
        void erase(K key) {
            _mutex.lock();
            tsCellPosition p = (*this)[key];
            if (p.exists) {
                memmove(&staticmap<K,V>::_map[p.position], &staticmap<K,V>::_map[p.position + 1], sizeof(typename staticmap<K,V>::Cell) * (staticmap<K,V>::_size - p.position));
                staticmap<K,V>::_size--;
            }
            _mutex.unlock();
        }
        void swap(tsstaticmap & o) {
            _mutex.lock();
            o._mutex.lock();
            typename staticmap<K,V>::Cell * map = staticmap<K,V>::_map;
            t_udword size = staticmap<K,V>::_size;
            t_udword capacity = staticmap<K,V>::_capacity;
            staticmap<K,V>::_map = o._map;
            staticmap<K,V>::_size = o._size;
            staticmap<K,V>::_capacity = o._capacity;
            o._map = map;
            o._size = size;
            o._capacity = capacity;
            o._mutex.unlock();
            _mutex.unlock();
        }
        void clear() {
            _mutex.lock();
            staticmap<K,V>::_size = 0;
            _mutex.unlock();
        }
        t_udword count(K key) {
            _mutex.lock();
            t_udword c = staticmap<K, V>::count(key);
            _mutex.unlock();
            return c;
        }
    protected:
        Mutex _mutex;
    };

    template<typename K, typename V>
    class tsdynamicmap : public dynamicmap<K,V> {
    private:
        Mutex _mutex;
    public:
        class tsCellPosition : public dynamicmap<K,V>::CellPosition {
        public:
            tsdynamicmap * map;
            V operator=(V x) {
                dynamicmap<K,V>::CellPosition::operator=(x);
                map->_mutex.unlock();
            }
            operator V () {
                try {
                    V x = dynamicmap<K,V>::CellPosition::operator V();
                    map->_mutex.unlock();
                    return x;
                }
                catch (MapError & e) {
                    map->_mutex.unlock();
                    throw;
                }
            }
        };

        tsdynamicmap() {
            dynamicmap<K,V>::_root = NULL;
            dynamicmap<K,V>::_size = 0;
        }
        tsdynamicmap(tsdynamicmap & o) {
            dynamicmap<K,V>::_root = new typename dynamicmap<K,V>::Node(*(o._root));
            dynamicmap<K,V>::_size = o._size;
        }
        virtual ~tsdynamicmap() {
            delete dynamicmap<K,V>::_root;
        }
        tsdynamicmap & operator=(tsdynamicmap & o) {
            delete dynamicmap<K,V>::_root;
            dynamicmap<K,V>::_root = new typename dynamicmap<K,V>::Node(*(o._root));
            dynamicmap<K,V>::_size = o._size;
        }
        bool empty() {
            _mutex.lock();
            bool b = dynamicmap<K,V>::_size == 0;
            _mutex.unlock();
            return b;
        }
        t_udword size() {
            _mutex.lock();
            t_udword s = dynamicmap<K,V>::_size;
            _mutex.unlock();
            return s;
        }
        tsCellPosition operator[](K key) {
            _mutex.lock();
            tsCellPosition p;
            typename dynamicmap<K,V>::Node * n = dynamicmap<K,V>::_root, * n_ant = NULL;
            while(n != NULL && n->key != key) {
                n_ant = n;
                if (n->key < key) { n = n->r; }
                else { n = n->l; }
            }
            p.map = this;
            p.exists = n != NULL;
            p.key = key;
            if (p.exists) { p.n = n; }
            else { p.n = n_ant; }
            return p;
        }
        void erase(K key) {
            _mutex.lock();
            try {
                dynamicmap<K,V>::erase(key);
                _mutex.unlock();
            }
            catch (MapError(e)) {
                _mutex.unlock();
                throw;
            }
        }
        void swap(tsdynamicmap & o) {
            _mutex.lock();
            o._mutex.lock();
            typename dynamicmap<K,V>::Node * tmproot = dynamicmap<K,V>::_root;
            t_udword tmpsize = dynamicmap<K,V>::_size;
            dynamicmap<K,V>::_root = o._root;
            dynamicmap<K,V>::_size = o._size;
            o._root = tmproot;
            o._size = tmpsize;
            o._mutex.unlock();
            _mutex.unlock();
        }
        void clear() {
            _mutex.lock();
            dynamicmap<K,V>::clear();
            _mutex.unlock();
        }
        t_udword count(K key) {
            _mutex.lock();
            t_udword c = dynamicmap<K,V>::count(key);
            _mutex.unlock();
            return c;
        }
    };

}

#ifdef _MSC_VER
    #pragma warning(disable: 4521 4522)
#endif

#endif //__TORUS_MAP_H
