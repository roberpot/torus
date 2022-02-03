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

#include <functional>
#include <memory>

#include <library/types.h>
#include <library/errors.h>
#include <threads/mutex.h>
#include <library/vector.h>
#include <library/stack.h>

#ifdef _MSC_VER
    #pragma warning(disable: 4521 4522)
#endif

#define _TTL_MAP_DEFAULT_SIZE 1

namespace ttl {

    template<class K, class T, class Compare = std::less<K>, class Allocator = std::allocator<std::pair<const K, T>>>
    class staticmap {
    public:
        typedef std::pair<const K, T> value_type;

        staticmap(udword_t size=_TTL_MAP_DEFAULT_SIZE);
        staticmap(const staticmap& o);
        staticmap(staticmap&& o);
        ~staticmap();
        staticmap& operator=(const staticmap& o);
        staticmap& operator=(staticmap&& o);

        // Element access.
        T& at(const K& key);
        T& operator[](const K& key);

        // Capacity.
        bool empty() const;
        bool size() const;
        bool capacity() const;
        bool max_size() const;

        // Modifiers.
        void clear();
        bool insert(const value_type& v);
        bool insert(value_type&& v);
        bool insert(const K& k, const T& t);
        bool insert(const K& k, T&& t);
        bool insert(K&& k, const T& t);
        bool insert(K&& k, T&& t);
        bool insert_or_assign(const value_type& v);
        bool insert_or_assign(value_type&& v);
        bool insert_or_assign(const K& k, const T& t);
        bool insert_or_assign(const K& k, T&& t);
        bool insert_or_assign(K&& k, const T& t);
        bool insert_or_assign(K&& k, T&& t);
        void erase(const K& k);
        void swap(staticmap& o);
        void merge(const staticmap& o);

        // Lookup.
        udword_t count(const K& k) const;
        vector<K> keys() const;

    private:
        udword_t _find(const K& k) const;
        void _insert_slice(udword_t p);
        value_type* _map;
        udword_t _size;
        udword_t _capacity;
        Compare _compare;
        Allocator _allocator;
    };

    template<class K, class T, class Compare = std::less<K>, class Allocator = std::allocator<std::pair<const K, T>>>
    class staticgrowingmap {
    public:
        typedef std::pair<const K, T> value_type;

        staticgrowingmap(udword_t size=_TTL_MAP_DEFAULT_SIZE);
        staticgrowingmap(const staticgrowingmap& o);
        staticgrowingmap(staticgrowingmap&& o);
        ~staticgrowingmap();
        staticgrowingmap& operator=(const staticgrowingmap& o);
        staticgrowingmap& operator=(staticgrowingmap&& o);

        // Element access.
        T& at(const K& key);
        T& operator[](const K& key);

        // Capacity.
        bool empty() const;
        bool size() const;
        bool capacity() const;
        bool max_size() const;

        // Modifiers.
        void clear();
        bool insert(const value_type& v);
        bool insert(value_type&& v);
        bool insert(const K& k, const T& t);
        bool insert(const K& k, T&& t);
        bool insert(K&& k, const T& t);
        bool insert(K&& k, T&& t);
        bool insert_or_assign(const value_type& v);
        bool insert_or_assign(value_type&& v);
        bool insert_or_assign(const K& k, const T& t);
        bool insert_or_assign(const K& k, T&& t);
        bool insert_or_assign(K&& k, const T& t);
        bool insert_or_assign(K&& k, T&& t);
        void erase(const K& k);
        void swap(staticgrowingmap& o);
        void merge(const staticgrowingmap& o);

        // Lookup.
        udword_t count(const K& k) const;
        vector<K> keys() const;

    private:
        udword_t _find(const K& k) const;
        void _insert_slice(udword_t p);
        value_type* _map;
        udword_t _size;
        udword_t _capacity;
        Compare _compare;
        Allocator _allocator;
    };

    template<class K, class T, class Compare = std::less<K>, class Allocator = std::allocator<std::pair<const K, T>>>
    class dynamicmap {
    public:
        typedef std::pair<const K, T> value_type;

        dynamicmap(udword_t size=_TTL_MAP_DEFAULT_SIZE);
        dynamicmap(const dynamicmap& o);
        dynamicmap(dynamicmap&& o);
        ~dynamicmap();
        dynamicmap& operator=(const dynamicmap& o);
        dynamicmap& operator=(dynamicmap&& o);

        // Element access.
        T& at(const K& key);
        T& operator[](const K& key);

        // Capacity.
        bool empty() const;
        bool size() const;
        bool capacity() const;
        bool max_size() const;

        // Modifiers.
        void clear();
        bool insert(const value_type& v);
        bool insert(value_type&& v);
        bool insert(const K& k, const T& t);
        bool insert(const K& k, T&& t);
        bool insert(K&& k, const T& t);
        bool insert(K&& k, T&& t);
        bool insert_or_assign(const value_type& v);
        bool insert_or_assign(value_type&& v);
        bool insert_or_assign(const K& k, const T& t);
        bool insert_or_assign(const K& k, T&& t);
        bool insert_or_assign(K&& k, const T& t);
        bool insert_or_assign(K&& k, T&& t);
        void erase(const K& k);
        void swap(dynamicmap& o);
        void merge(const dynamicmap& o);

        // Lookup.
        udword_t count(const K& k) const;
        vector<K> keys() const;

    private:
        struct Node {
            enum Color {RED, BLACK} color;
            value_type v;
            Node* parent;
            Node* left;
            Node* right;

            Node(Node* p, const K& k);
            ~Node();
            Node* grandfather() const;
            Node* uncle() const;
            Node* sibling() const;
            void rotate_left(Node** root);
            void rotate_right(Node** root);
            bool is_on_tree() const;
            static bool is_black(Node* n);
            static bool is_red(Node* n);
        };
        Node* _find(const K& k) const;
        void _insert_slice(Node* node);
        value_type* _map;
        udword_t _size;
        Compare _compare;
        Allocator _allocator;
    };

    /**
     * Implementation.
     */

    template<class K, class T, class Compare, class Allocator>
    staticmap<K,T,Compare,Allocator>::staticmap(udword_t size) :
            _size(0), _capacity(size) {
        _map = _allocator.allocate(_capacity);
    }

    template<class K, class T, class Compare, class Allocator>
    staticmap<K,T,Compare,Allocator>::staticmap(const staticmap& o) :
            _size(o._size), _capacity(o._capacity) {
        _map = _allocator.allocate(_capacity);
        for (udword_t i = 0; i < _size; ++i) {
            _allocator.construct(&(_map[i]), o._map[i]);
        }
    }

    template<class K, class T, class Compare, class Allocator>
    staticmap<K,T,Compare,Allocator>::staticmap(staticmap&& o) {
        _map = o._map;
        _size = o._size;
        _capacity = o._capacity;
        o._map = nullptr;
        o._size = 0;
        o._capacity = 0;
    }

    template<class K, class T, class Compare, class Allocator>
    staticmap<K,T,Compare,Allocator>::~staticmap() {
        if (nullptr != _map) {
            clear();
            _allocator.deallocate(_map, _capacity);
        }
    }

    template<class K, class T, class Compare, class Allocator>
    staticmap<K,T,Compare,Allocator>& staticmap<K,T,Compare,Allocator>::operator=(const staticmap& o) {
        if (this != &o) {
            value_type* aux = _map;
            udword_t aux_size = _size;
            udword_t aux_capacity = _capacity;
            _map = o._map;
            _size = o._size;
            _capacity = o._capacity;
            o._map = aux;
            o._size = aux_size;
            o._capacity = aux_capacity;
        }
        return *this;
    }

    template<class K, class T, class Compare, class Allocator>
    staticmap<K,T,Compare,Allocator>& staticmap<K,T,Compare,Allocator>::operator=(staticmap&& o) {
        if (this != &o) {
            value_type* aux = _map;
            udword_t aux_size = _size;
            udword_t aux_capacity = _capacity;
            _map = o._map;
            _size = o._size;
            _capacity = o._capacity;
            o._map = aux;
            o._size = aux_size;
            o._capacity = aux_capacity;
        }
        return *this;
    }

    template<class K, class T, class Compare, class Allocator>
    T& staticmap<K,T,Compare,Allocator>::at(const K& key) {
        udword_t p = _find(key);
        if (Compare(_map[p].first, key) || Compare(key, _map[p].first)) {
            throw MapError("Key not found on map");
        }
        return _map[p].second;
    }

    template<class K, class T, class Compare, class Allocator>
    T& staticmap<K,T,Compare,Allocator>::operator[](const K& key) {
        udword_t p = _find(key);
        if (_compare(_map[p].first, key) || _compare(key, _map[p].first)) {
            if (_size == _capacity) {
                throw MapError("No space on map");
            }
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(key, T()));
        }
        return _map[p].second;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::empty() const {
        return _size == 0;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::size() const {
        return _size;

    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::capacity() const {
        return _capacity;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::max_size() const {
        return 0;
    }

    template<class K, class T, class Compare, class Allocator>
    void staticmap<K,T,Compare,Allocator>::clear() {
        for (udword_t i = 0; i < _size; ++i) {
            _allocator.destroy(&(_map[i]));
        }
        _size = 0;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::insert(const value_type& v) {
        udword_t p = _find(v.first);
        if (_compare(_map[p].first, v.first) || _compare(v.first, _map[p].first)) {
            if (_size == _capacity) {
                throw MapError("No space on map");
            }
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), v);
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::insert(value_type&& v) {
        udword_t p = _find(v.first);
        if (_compare(_map[p].first, v.first) || _compare(v.first, _map[p].first)) {
            if (_size == _capacity) {
                throw MapError("No space on map");
            }
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), v);
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::insert(const K& k, const T& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            if (_size == _capacity) {
                throw MapError("No space on map");
            }
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::insert(const K& k, T&& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            if (_size == _capacity) {
                throw MapError("No space on map");
            }
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::insert(K&& k, const T& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            if (_size == _capacity) {
                throw MapError("No space on map");
            }
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::insert(K&& k, T&& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            if (_size == _capacity) {
                throw MapError("No space on map");
            }
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::insert_or_assign(const value_type& v) {
        udword_t p = _find(v.first);
        if (_compare(_map[p].first, v.first) || _compare(v.first, _map[p].first)) {
            if (_size == _capacity) {
                throw MapError("No space on map");
            }
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), v);
            return true;
        } else {
            _map[p] = v;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::insert_or_assign(value_type&& v) {
        udword_t p = _find(v.first);
        if (_compare(_map[p].first, v.first) || _compare(v.first, _map[p].first)) {
            if (_size == _capacity) {
                throw MapError("No space on map");
            }
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), v);
            return true;
        } else {
            _map[p] = v;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::insert_or_assign(const K& k, const T& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            if (_size == _capacity) {
                throw MapError("No space on map");
            }
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        } else {
            _map[p] = std::make_pair(k, t);
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::insert_or_assign(const K& k, T&& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            if (_size == _capacity) {
                throw MapError("No space on map");
            }
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        } else {
            _map[p] = std::make_pair(k, t);
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::insert_or_assign(K&& k, const T& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            if (_size == _capacity) {
                throw MapError("No space on map");
            }
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        } else {
            _map[p] = std::make_pair(k, t);
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticmap<K,T,Compare,Allocator>::insert_or_assign(K&& k, T&& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            if (_size == _capacity) {
                throw MapError("No space on map");
            }
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        } else {
            _map[p] = std::make_pair(k, t);
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    void staticmap<K,T,Compare,Allocator>::erase(const K& k) {
        udword_t p = _find(k);
        if (!(_compare(_map[p].first, k) || _compare(k, _map[p].first))) {
            _allocator.destroy(&(_map[p]));
            udword_t len = _size - p - 1;
            if (len > 0) {
                value_type* aux = _allocator.allocate(len);
                memcpy(aux, &(_map[p + 1]), sizeof(value_type) * len);
                memcpy(&(_map[p]), aux, sizeof(value_type) * len);
                _allocator.deallocate(aux, len);
            }
            _size--;
        }
    }

    template<class K, class T, class Compare, class Allocator>
    void staticmap<K,T,Compare,Allocator>::swap(staticmap& o) {
        if (this != &o) {
            value_type* aux = _map;
            udword_t aux_size = _size;
            udword_t aux_capacity = _capacity;
            _map = o._map;
            _size = o._size;
            _capacity = o._capacity;
            o._map = aux;
            o._size = aux_size;
            o._capacity = aux_capacity;
        }
    }

    template<class K, class T, class Compare, class Allocator>
    void staticmap<K,T,Compare,Allocator>::merge(const staticmap& o) {
        for(udword_t i = 0; i < o._size; i++) {
            insert_or_assign(o._map[i]);
        }
    }

    template<class K, class T, class Compare, class Allocator>
    udword_t staticmap<K,T,Compare,Allocator>::_find(const K& k) const {
        dword_t init;
        dword_t end;
        udword_t mid;
        init = 0;
        end = _size - 1;
        while (! _compare(end, init)) {
            mid = (init + end) / 2;
            if (_compare(_map[mid].first, k)) {
                init = mid + 1;
            } else if (_compare(k, _map[mid].first)) {
                end = mid - 1;
            } else {
                return mid;
            }
        }
        return static_cast<udword_t>(init);
    }

    template<class K, class T, class Compare, class Allocator>
    void staticmap<K,T,Compare,Allocator>::_insert_slice(udword_t p) {
        if (_size == _capacity) {
            throw MapError("No space on map");
        }
        udword_t len = _size - p;
        if (len > 0) {
            value_type* aux = _allocator.allocate(len);
            memcpy(aux, &(_map[p]), sizeof(value_type) * len);
            memcpy(&(_map[p+1]), aux, sizeof(value_type) * len);
            _allocator.deallocate(aux, len);
        }
    }

    template<class K, class T, class Compare, class Allocator>
    udword_t staticmap<K,T,Compare,Allocator>::count(const K& k) const {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            return 0;
        }
        return 1;
    }

    template<class K, class T, class Compare, class Allocator>
    vector<K> staticmap<K,T,Compare,Allocator>::keys() const {
        vector<K> keys;
        for (udword_t i = 0; i < _size; ++i) {
            keys.push_back(_map[i].first);
        }
        return keys;
    }


    template<class K, class T, class Compare, class Allocator>
    staticgrowingmap<K,T,Compare,Allocator>::staticgrowingmap(udword_t size) :
        _size(0), _capacity(size) {
        _map = _allocator.allocate(_capacity);
    }

    template<class K, class T, class Compare, class Allocator>
    staticgrowingmap<K,T,Compare,Allocator>::staticgrowingmap(const staticgrowingmap& o) :
        _size(o._size), _capacity(o._capacity) {
        _map = _allocator.allocate(_capacity);
        for (udword_t i = 0; i < _size; ++i) {
            _allocator.construct(&(_map[i]), o._map[i]);
        }
    }

    template<class K, class T, class Compare, class Allocator>
    staticgrowingmap<K,T,Compare,Allocator>::staticgrowingmap(staticgrowingmap&& o) {
        _map = o._map;
        _size = o._size;
        _capacity = o._capacity;
        o._map = nullptr;
        o._size = 0;
        o._capacity = 0;
    }

    template<class K, class T, class Compare, class Allocator>
    staticgrowingmap<K,T,Compare,Allocator>::~staticgrowingmap() {
        if (nullptr != _map) {
            clear();
            _allocator.deallocate(_map, _capacity);
        }
    }

    template<class K, class T, class Compare, class Allocator>
    staticgrowingmap<K,T,Compare,Allocator>& staticgrowingmap<K,T,Compare,Allocator>::operator=(const staticgrowingmap& o) {
        if (this != &o) {
            value_type* aux = _map;
            udword_t aux_size = _size;
            udword_t aux_capacity = _capacity;
            _map = o._map;
            _size = o._size;
            _capacity = o._capacity;
            o._map = aux;
            o._size = aux_size;
            o._capacity = aux_capacity;
        }
        return *this;
    }

    template<class K, class T, class Compare, class Allocator>
    staticgrowingmap<K,T,Compare,Allocator>& staticgrowingmap<K,T,Compare,Allocator>::operator=(staticgrowingmap&& o) {
        if (this != &o) {
            value_type* aux = _map;
            udword_t aux_size = _size;
            udword_t aux_capacity = _capacity;
            _map = o._map;
            _size = o._size;
            _capacity = o._capacity;
            o._map = aux;
            o._size = aux_size;
            o._capacity = aux_capacity;
        }
        return *this;
    }

    template<class K, class T, class Compare, class Allocator>
    T& staticgrowingmap<K,T,Compare,Allocator>::at(const K& key) {
        udword_t p = _find(key);
        if (Compare(_map[p].first, key) || Compare(key, _map[p].first)) {
            throw MapError("Key not found on map");
        }
        return _map[p].second;
    }

    template<class K, class T, class Compare, class Allocator>
    T& staticgrowingmap<K,T,Compare,Allocator>::operator[](const K& key) {
        udword_t p = _find(key);
        if (_compare(_map[p].first, key) || _compare(key, _map[p].first)) {
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(key, T()));
        }
        return _map[p].second;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::empty() const {
        return _size == 0;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::size() const {
        return _size;

    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::capacity() const {
        return _capacity;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::max_size() const {
        return 0;
    }

    template<class K, class T, class Compare, class Allocator>
    void staticgrowingmap<K,T,Compare,Allocator>::clear() {
        for (udword_t i = 0; i < _size; ++i) {
            _allocator.destroy(&(_map[i]));
        }
        _size = 0;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::insert(const value_type& v) {
        udword_t p = _find(v.first);
        if (_compare(_map[p].first, v.first) || _compare(v.first, _map[p].first)) {
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), v);
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::insert(value_type&& v) {
        udword_t p = _find(v.first);
        if (_compare(_map[p].first, v.first) || _compare(v.first, _map[p].first)) {
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), v);
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::insert(const K& k, const T& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::insert(const K& k, T&& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::insert(K&& k, const T& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::insert(K&& k, T&& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::insert_or_assign(const value_type& v) {
        udword_t p = _find(v.first);
        if (_compare(_map[p].first, v.first) || _compare(v.first, _map[p].first)) {
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), v);
            return true;
        } else {
            _map[p] = v;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::insert_or_assign(value_type&& v) {
        udword_t p = _find(v.first);
        if (_compare(_map[p].first, v.first) || _compare(v.first, _map[p].first)) {
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), v);
            return true;
        } else {
            _map[p] = v;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::insert_or_assign(const K& k, const T& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        } else {
            _map[p] = std::make_pair(k, t);
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::insert_or_assign(const K& k, T&& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        } else {
            _map[p] = std::make_pair(k, t);
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::insert_or_assign(K&& k, const T& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        } else {
            _map[p] = std::make_pair(k, t);
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool staticgrowingmap<K,T,Compare,Allocator>::insert_or_assign(K&& k, T&& t) {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            _insert_slice(p);
            _allocator.allocate(&(_map[p]), std::make_pair(k, t));
            return true;
        } else {
            _map[p] = std::make_pair(k, t);
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    void staticgrowingmap<K,T,Compare,Allocator>::erase(const K& k) {
        udword_t p = _find(k);
        if (!(_compare(_map[p].first, k) || _compare(k, _map[p].first))) {
            _allocator.destroy(&(_map[p]));
            udword_t len = _size - p - 1;
            if (len > 0) {
                value_type* aux = _allocator.allocate(len);
                memcpy(aux, &(_map[p + 1]), sizeof(value_type) * len);
                memcpy(&(_map[p]), aux, sizeof(value_type) * len);
                _allocator.deallocate(aux, len);
            }
            _size--;
        }
    }

    template<class K, class T, class Compare, class Allocator>
    void staticgrowingmap<K,T,Compare,Allocator>::swap(staticgrowingmap& o) {
        if (this != &o) {
            value_type* aux = _map;
            udword_t aux_size = _size;
            udword_t aux_capacity = _capacity;
            _map = o._map;
            _size = o._size;
            _capacity = o._capacity;
            o._map = aux;
            o._size = aux_size;
            o._capacity = aux_capacity;
        }
    }

    template<class K, class T, class Compare, class Allocator>
    void staticgrowingmap<K,T,Compare,Allocator>::merge(const staticgrowingmap& o) {
        for(udword_t i = 0; i < o._size; i++) {
            insert_or_assign(o._map[i]);
        }
    }

    template<class K, class T, class Compare, class Allocator>
    udword_t staticgrowingmap<K,T,Compare,Allocator>::_find(const K& k) const {
        dword_t init;
        dword_t end;
        udword_t mid;
        init = 0;
        end = _size - 1;
        while (! _compare(end, init)) {
            mid = (init + end) / 2;
            if (_compare(_map[mid].first, k)) {
                init = mid + 1;
            } else if (_compare(k, _map[mid].first)) {
                end = mid - 1;
            } else {
                return mid;
            }
        }
        return static_cast<udword_t>(init);
    }

    template<class K, class T, class Compare, class Allocator>
    void staticgrowingmap<K,T,Compare,Allocator>::_insert_slice(udword_t p) {
        if (_size == _capacity) {
            udword_t newcapacity = 1;
            while (newcapacity < _capacity) {
                newcapacity = newcapacity << 1;
            }
            value_type* aux = _allocator.allocate(newcapacity);
            memcpy(aux, _map, sizeof(value_type) * _capacity);
            _allocator.deallocate(_map, _capacity);
            _map = aux;
            _capacity = newcapacity;
        }
        udword_t len = _size - p;
        if (len > 0) {
            value_type* aux = _allocator.allocate(len);
            memcpy(aux, &(_map[p]), sizeof(value_type) * len);
            memcpy(&(_map[p+1]), aux, sizeof(value_type) * len);
            _allocator.deallocate(aux, len);
        }
    }

    template<class K, class T, class Compare, class Allocator>
    udword_t staticgrowingmap<K,T,Compare,Allocator>::count(const K& k) const {
        udword_t p = _find(k);
        if (_compare(_map[p].first, k) || _compare(k, _map[p].first)) {
            return 0;
        }
        return 1;
    }

    template<class K, class T, class Compare, class Allocator>
    vector<K> staticgrowingmap<K,T,Compare,Allocator>::keys() const {
        vector<K> keys;
        for (udword_t i = 0; i < _size; ++i) {
            keys.push_back(_map[i].first);
        }
        return keys;
    }

    template<class K, class T, class Compare, class Allocator>
    dynamicmap<K,T,Compare,Allocator>::dynamicmap(udword_t size) : _map(nullptr), _size(0) {
        UNREFERENCED_PARAMETER(size);
    }

    template<class K, class T, class Compare, class Allocator>
    dynamicmap<K,T,Compare,Allocator>::dynamicmap(const dynamicmap& o) : _map(nullptr), _size(o._size){
        dynamicstack<Node*> stack;
        Node* aux = o._map;
        do {
            if (false == stack.empty() && nullptr == aux) {
                insert(stack.top()->v);
            }
            if (nullptr != aux) {
                stack.push(aux);
                aux = aux->left;
            } else if (false == stack.empty()) {
                aux = stack.top();
                stack.pop();
                aux = aux->right;
            }
        } while (false == stack.empty() || nullptr != aux);
    }

    template<class K, class T, class Compare, class Allocator>
    dynamicmap<K,T,Compare,Allocator>::dynamicmap(dynamicmap&& o) {
        _map = o._map;
        _size = o._size;
        o._map = nullptr;
        o._size = 0;
    }

    template<class K, class T, class Compare, class Allocator>
    dynamicmap<K,T,Compare,Allocator>::~dynamicmap() {
        clear();
    }

    template<class K, class T, class Compare, class Allocator>
    dynamicmap<K,T,Compare,Allocator>& dynamicmap<K,T,Compare,Allocator>::operator=(const dynamicmap& o) {
        if (this != &o) {
            clear();
            dynamicstack<Node*> stack;
            Node* aux = o._map;
            do {
                if (false == stack.empty() && nullptr == aux) {
                    insert(stack.top()->v);
                }
                if (nullptr != aux) {
                    stack.push(aux);
                    aux = aux->left;
                } else if (false == stack.empty()) {
                    aux = stack.top();
                    stack.pop();
                    aux = aux->right;
                }
            } while (false == stack.empty() || nullptr != aux);
            _size = o._size;
        }
    }

    template<class K, class T, class Compare, class Allocator>
    dynamicmap<K,T,Compare,Allocator>& dynamicmap<K,T,Compare,Allocator>::operator=(dynamicmap&& o) {
        if (this != &o) {
            swap(o);
        }
        return *this;
    }

    template<class K, class T, class Compare, class Allocator>
    T& dynamicmap<K,T,Compare,Allocator>::at(const K& key) {
        Node* n = _find(key);
        if (false == n->is_on_tree()) {
            throw MapError("Key not found on map");
        }
        return n->v.second;
    }

    template<class K, class T, class Compare, class Allocator>
    T& dynamicmap<K,T,Compare,Allocator>::operator[](const K& key) {
        Node* n = _find(key);
        if (false == n->is_on_tree()) {
            _insert_slice(n);
            _size++;
        }
        return n->v.second;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::empty() const {
        return _size == 0;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::size() const {
        return _size;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::capacity() const{
        return 0;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::max_size() const {
        return 0;
    }

    template<class K, class T, class Compare, class Allocator>
    void dynamicmap<K,T,Compare,Allocator>::clear() {
        if (nullptr != _map) {
            delete _map;
            _map = nullptr;
        }
        _size = 0;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::insert(const value_type& v) {
        Node * n = _find(v.first);
        if (false == n->is_on_tree()) {
            _insert_slice(n);
            n->v = v;
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::insert(value_type&& v) {
        Node * n = _find(v.first);
        if (false == n->is_on_tree()) {
            _insert_slice(n);
            n->v = v;
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::insert(const K& k, const T& t) {
        Node * n = _find(k);
        if (false == n->is_on_tree()) {
            _insert_slice(n);
            n->v.first = k;
            n->v.second = t;
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::insert(const K& k, T&& t) {
        Node * n = _find(k);
        if (false == n->is_on_tree()) {
            _insert_slice(n);
            n->v.first = k;
            n->v.second = t;
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::insert(K&& k, const T& t) {
        Node * n = _find(k);
        if (false == n->is_on_tree()) {
            _insert_slice(n);
            n->v.first = k;
            n->v.second = t;
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::insert(K&& k, T&& t) {
        Node * n = _find(k);
        if (false == n->is_on_tree()) {
            _insert_slice(n);
            n->v.first = k;
            n->v.second = t;
            return true;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::insert_or_assign(const value_type& v) {
        Node * n = _find(v.first);
        if (false == n->is_on_tree()) {
            _insert_slice(n);
            n->v = v;
            return true;
        } else {
            n->v = v;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::insert_or_assign(value_type&& v) {
        Node * n = _find(v.first);
        if (false == n->is_on_tree()) {
            _insert_slice(n);
            n->v = v;
            return true;
        } else {
            n->v = v;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::insert_or_assign(const K& k, const T& t) {
        Node * n = _find(k);
        if (false == n->is_on_tree()) {
            _insert_slice(n);
            n->v.first = k;
            n->v.second = t;
            return true;
        } else {
            n->v.second = t;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::insert_or_assign(const K& k, T&& t) {
        Node * n = _find(k);
        if (false == n->is_on_tree()) {
            _insert_slice(n);
            n->v.first = k;
            n->v.second = t;
            return true;
        } else {
            n->v.second = t;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::insert_or_assign(K&& k, const T& t) {
        Node * n = _find(k);
        if (false == n->is_on_tree()) {
            _insert_slice(n);
            n->v.first = k;
            n->v.second = t;
            return true;
        } else {
            n->v.second = t;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::insert_or_assign(K&& k, T&& t) {
        Node * n = _find(k);
        if (false == n->is_on_tree()) {
            _insert_slice(n);
            n->v.first = k;
            n->v.second = t;
            return true;
        } else {
            n->v.second = t;
        }
        return false;
    }

    template<class K, class T, class Compare, class Allocator>
    void dynamicmap<K,T,Compare,Allocator>::erase(const K& k) {
        Node* n = _find(k);

        if (false == n->is_on_tree()) {
            throw MapError("Key not found.");
        }
        if (nullptr != n->left && nullptr != n->right) {
            Node* o = n->right;
            while (nullptr != o->left) {
                o = o->left;
            }
            n->v = o->v;
            n = o;
        }
        Node* c;
        if (nullptr != n->left) {
            c = n->left;
        } else {
            c = n->right;
        }
        // Get parent.
        Node* p = n->parent;
        // Replace node with child.
        if (nullptr != c) {
            c->parent = p;
        }
        if (nullptr != p && p->left == n) {
            p->left = c;
        } else if (nullptr != p && p->right == n) {
            p->right = c;
        }
        // Base case.
        if (Node::is_black(n) && Node::is_red(c)) {
            c->color = Node::BLACK;
        } else if (Node::is_black(n) && Node::is_black(c)) {
            // Case 1. Is root node.
            if (nullptr == p) {
                _map = c;
            } else {
                // Get sibling.
                Node* s;
                if (p->left == c) {
                    s = p->right;
                } else {
                    s = p->left;
                }
                // Case 2. Sibling is red.
                if (Node::is_red(s)) {
                    s->color = Node::BLACK;
                    p->color = Node::RED;
                    if (c == p->left) {
                        p->rotate_left(_map);
                    } else {
                        p->rotate_right(_map);
                    }
                }
                // Case 3.
            }
        }
        // Remove node.
        n->left = nullptr;
        n->right = nullptr;
        delete n;
    }

    template<class K, class T, class Compare, class Allocator>
    void dynamicmap<K,T,Compare,Allocator>::swap(dynamicmap& o) {
        if (this != &o) {
            Node* aux = _map;
            udword_t aux_size = _size;
            _map = o._map;
            _size = o._size;
            o._map = aux;
            o._size = aux_size;
        }
    }

    template<class K, class T, class Compare, class Allocator>
    void dynamicmap<K,T,Compare,Allocator>::merge(const dynamicmap& o) {

    }

    template<class K, class T, class Compare, class Allocator>
    typename dynamicmap<K,T,Compare,Allocator>::Node* dynamicmap<K,T,Compare,Allocator>::_find(const K& k) const {
        Node* node = _map;
        Node* parent = nullptr;
        while (nullptr != node && (_compare(k, node->v.first) || _compare(node->v.first, k))) {
            parent = node;
            if (_compare(k, node->v.first)) {
                node = node->left;
            } else if (_compare(node->v.first, k)) {
                node = node->right;
            }
        }
        if (node == nullptr) {
            node = new Node(parent, k);
        }
        return node;
    }

    template<class K, class T, class Compare, class Allocator>
    void dynamicmap<K,T,Compare,Allocator>::_insert_slice(Node* node) {
        if (nullptr == node->parent) {
            _map = node;
        } else if (_compare(node->v.first, node->parent->v.first)) {
            node->parent->left = node;
        } else if (_compare(node->parent->v.first, node->v.first)) {
            node->parent->right = node;
        } else {
            MapError("Key duped");
        }
        node->color = Node::RED;
        Node* current_node;
        Node* next_node = node;
        while (nullptr != (current_node = next_node)) {
            next_node = nullptr;
            // Case 1.
            if (nullptr == current_node->parent) {
                current_node->color = Node::BLACK;
                continue;
            }
            // Case 2.
            if (Node::is_black(current_node->parent)) {
                continue;
            }
            // Case 3.
            Node* uncle = current_node->uncle();
            Node* grandfather = current_node->grandfather();
            if (Node::is_red(uncle)) {
                current_node->parent->color = Node::BLACK;
                uncle->color = Node::BLACK;
                grandfather->color = Node::RED;
                next_node = grandfather;
                continue;
            }
            // Case 4.
            if (current_node->parent->right == current_node && current_node->parent == grandfather->left) {
                current_node->parent->rotate_left(_map);
                current_node = current_node->left;
            } else if (current_node->parent->left == current_node && current_node->parent == grandfather->right) {
                current_node->parent->rotate_right(_map);
                current_node = current_node->right;
            }
            current_node->parent->color = Node::BLACK;
            grandfather->color = Node::RED;
            if (current_node == current_node->parent->left) {
                grandfather->rotate_right(_map);
            } else {
                grandfather->rotate_left(_map);
            }
        }
    }

    template<class K, class T, class Compare, class Allocator>
    dynamicmap<K,T,Compare,Allocator>::Node::Node(Node* p, const K& k) : parent(p), left(nullptr), right(nullptr) {
        v.first = k;
    }

    template<class K, class T, class Compare, class Allocator>
    dynamicmap<K,T,Compare,Allocator>::Node::~Node(){
        if (nullptr != left) {
            delete left;
        }
        if (nullptr != right) {
            delete right;
        }
    }

    template<class K, class T, class Compare, class Allocator>
    typename dynamicmap<K,T,Compare,Allocator>::Node* dynamicmap<K,T,Compare,Allocator>::Node::grandfather() const {
        if (nullptr == parent) {
            return nullptr;
        }
        return parent->parent;
    }

    template<class K, class T, class Compare, class Allocator>
    typename dynamicmap<K,T,Compare,Allocator>::Node* dynamicmap<K,T,Compare,Allocator>::Node::uncle() const {
        Node* grandfather = grandfather();
        if (nullptr == grandfather) {
            return nullptr;
        }
        if (parent == grandfather->left) {
            return grandfather->right;
        } else {
            return grandfather->left;
        }
    }

    template<class K, class T, class Compare, class Allocator>
    typename dynamicmap<K,T,Compare,Allocator>::Node* dynamicmap<K,T,Compare,Allocator>::Node::sibling() const {
        if (nullptr == parent) {
            return nullptr;
        }
        if (this == parent->left) {
            return parent->right;
        } else {
            return parent->left;
        }
    }

    template<class K, class T, class Compare, class Allocator>
    void dynamicmap<K,T,Compare,Allocator>::Node::rotate_left(Node** root) {
        Node* a;
        Node* b;
        Node* c;
        Node** p = root;
        if (nullptr != parent && this == parent->right) {
            p = &(parent->right);
        } else if (nullptr != parent && this == parent->left) {
            p = &(parent->left);
        }
        *p = right;
        (*p)->parent = parent;
        parent = *p;
        right = (*p)->left;
        (*p)->left = this;

        if (nullptr != right) {
            right->parent = this;
        }
    }

    template<class K, class T, class Compare, class Allocator>
    void dynamicmap<K,T,Compare,Allocator>::Node::rotate_right(Node** root) {
        Node* a;
        Node* b;
        Node* c;
        Node** p = root;
        if (nullptr != parent && this == parent->right) {
            p = &(parent->right);
        } else if (nullptr != parent && this == parent->left) {
            p = &(parent->left);
        }
        *p = left;
        (*p)->parent = parent;
        parent = *p;
        left = (*p)->right;
        (*p)->right = this;

        if (nullptr != left) {
            left->parent = this;
        }
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::Node::is_on_tree() const {
        if (nullptr == parent) {
            return false;
        }
        return parent->left == this || parent->right == this;
    }
    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::Node::is_black(Node* n) {
        return nullptr == n || Node::BLACK == n->color;
    }

    template<class K, class T, class Compare, class Allocator>
    bool dynamicmap<K,T,Compare,Allocator>::Node::is_red(Node* n) {
        return nullptr != n && Node::RED == n->color;
    }





    ////////////////////// OLD


    template<typename K, typename V>
    class __old__staticmap {
    public:
        class CellPosition {
        public:
            __old__staticmap * map;
            bool exists;
            K key;
            udword_t position;
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

        __old__staticmap(udword_t size=_TTL_MAP_DEFAULT_SIZE) {
            _capacity = size;
            _size = 0;
            _map = new Cell[_capacity];
        }
        __old__staticmap(__old__staticmap & o) {
            _capacity = o._capacity;
            _size = o._size;
            _map = new Cell[_capacity];
            memcpy(_map, o._map, sizeof(Cell) * _capacity);
        }
        virtual ~__old__staticmap() {
            delete[] _map;
        }
        __old__staticmap & operator=(__old__staticmap & o) {
            delete _map;
            _capacity = o._capacity;
            _size = o._size;
            _map = new Cell[_capacity];
            memcpy(_map, o._map, sizeof(Cell) * _capacity);
            return *this;
        }
        virtual bool empty() { return _size == 0; }
        virtual udword_t size() { return _size; }
        virtual udword_t max_size() { return TDWORD_MAX; }
        CellPosition operator[](K key) {
            CellPosition p;
            udword_t init = 0, end = _size - 1, pivot = 0;
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
        void swap(__old__staticmap & o) {
            Cell * map = _map;
            udword_t size = _size;
            udword_t capacity = _capacity;
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
        virtual udword_t count(K key) {
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
        virtual void _reserve(udword_t n) {
            if (_capacity < n) {
                _resize(n + _TTL_MAP_DEFAULT_SIZE);
            }
        }
        virtual void _resize(udword_t s) {
            Cell * n = new Cell[s];
            udword_t nsize = _size;
            if (s < _size) nsize = s;
            memcpy(n, _map, sizeof(Cell) * nsize);
            _size = nsize;
            _capacity = s;
            delete _map;
            _map = n;
        }
        Cell * _map;
        udword_t _capacity;
        udword_t _size;
    };

    template<typename K, typename V>
    class __old_dynamicmap {
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
            udword_t child_count() {
                udword_t count = 0;
                if (l != NULL) count++;
                if (r != NULL) count++;
                return count;
            }
            void replace_by_child() {
                if (r != NULL && l != NULL) {
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
        udword_t _size;
    public:
        class CellPosition {
        public:
            __old_dynamicmap * map;
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

        __old_dynamicmap() {
            _root = NULL;
            _size = 0;
        }
        __old_dynamicmap(__old_dynamicmap & o) {
            _root = new Node(*(o._root));
            _size = o._size;
        }
        virtual ~__old_dynamicmap() {
            delete _root;
        }
        __old_dynamicmap & operator=(__old_dynamicmap & o) {
            delete _root;
            _root = new Node(*(o._root));
            _size = o._size;
        }
        virtual bool empty() { return _size == 0; }
        virtual udword_t size() { return _size; }
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
            udword_t childs = n->child_count();
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
        void swap(__old_dynamicmap & o) {
            Node * tmproot = _root;
            udword_t tmpsize = _size;
            _root = o._root;
            _size = o._size;
            o._root = tmproot;
            o._size = tmpsize;
        }
        virtual void clear() {
            delete _root;
            _size = 0;
        }
        virtual udword_t count(K key) {
            CellPosition p = (*this)[key];
            if (p.exists) { return 1; }
            return 0;
        }
    };

    template<typename K, typename V>
    class __old_tsstaticmap : public __old__staticmap<K, V> {
    public:
        class tsCellPosition : public __old__staticmap<K, V>::CellPosition {
        public:
            __old_tsstaticmap * map;
            bool exists;
            K key;
            udword_t position;
            V operator=(V x) {
                tsCellPosition::operator=(x);
                map->_mutex.unlock();
                return x;
            }
            operator V () {
                try {
                    V x = __old__staticmap<K,V>::CellPosition::operator V();
                    map->_mutex.unlock();
                    return x;
                }
                catch (MapError & e) {
                    map->_mutex.unlock();
                    throw;
                }
            }
        };

        __old_tsstaticmap(udword_t size=_TTL_MAP_DEFAULT_SIZE) {
            __old__staticmap<K,V>::_capacity = size;
            __old__staticmap<K,V>::_size = 0;
            __old__staticmap<K,V>::_map = new typename __old__staticmap<K,V>::Cell[__old__staticmap<K,V>::_capacity];
        }
        __old_tsstaticmap(__old_tsstaticmap & o) {
            o._mutex.lock();
            __old__staticmap<K,V>::_capacity = o._capacity;
            __old__staticmap<K,V>::_size = o._size;
            __old__staticmap<K,V>::_map = new typename __old__staticmap<K,V>::Cell[__old__staticmap<K,V>::_capacity];
            memcpy(__old__staticmap<K,V>::_map, o._map, sizeof(typename __old__staticmap<K,V>::Cell) * __old__staticmap<K,V>::_capacity);
            o._mutex.unlock();
        }
        virtual ~__old_tsstaticmap() {
            delete __old__staticmap<K,V>::_map;
        }
        __old_tsstaticmap & operator=(__old_tsstaticmap & o) {
            _mutex.lock();
            o._mutex.lock();
            delete __old__staticmap<K,V>::_map;
            __old__staticmap<K,V>::_capacity = o._capacity;
            __old__staticmap<K,V>::_size = o._size;
            __old__staticmap<K,V>::_map = new typename __old__staticmap<K,V>::Cell[__old__staticmap<K,V>::_capacity];
            memcpy(__old__staticmap<K,V>::_map, o._map, sizeof(typename __old__staticmap<K,V>::Cell) * __old__staticmap<K,V>::_capacity);
            o._mutex.unlock();
            _mutex.unlock();
            return *this;
        }
        bool empty() {
            _mutex.lock();
            bool b = __old__staticmap<K,V>::_size == 0;
            _mutex.unlock();
            return b;
        }
        udword_t size() {
            _mutex.lock();
            udword_t s = __old__staticmap<K,V>::_size;
            _mutex.unlock();
            return s;
        }
        udword_t max_size() { return TDWORD_MAX; }
        tsCellPosition operator[](K key) {
            _mutex.lock();
            tsCellPosition p;
            udword_t init = 0, end = __old__staticmap<K,V>::_size - 1, pivot = 0;
            K pivot_key;
            bool stop = false;
            while ( init > end && !stop) {
                pivot = (end - init) / 2 + init;
                pivot_key = __old__staticmap<K,V>::_map[pivot].key;
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
                memmove(&__old__staticmap<K,V>::_map[p.position], &__old__staticmap<K,V>::_map[p.position + 1], sizeof(typename __old__staticmap<K,V>::Cell) * (__old__staticmap<K,V>::_size - p.position));
                __old__staticmap<K,V>::_size--;
            }
            _mutex.unlock();
        }
        void swap(__old_tsstaticmap & o) {
            _mutex.lock();
            o._mutex.lock();
            typename __old__staticmap<K,V>::Cell * map = __old__staticmap<K,V>::_map;
            udword_t size = __old__staticmap<K,V>::_size;
            udword_t capacity = __old__staticmap<K,V>::_capacity;
            __old__staticmap<K,V>::_map = o._map;
            __old__staticmap<K,V>::_size = o._size;
            __old__staticmap<K,V>::_capacity = o._capacity;
            o._map = map;
            o._size = size;
            o._capacity = capacity;
            o._mutex.unlock();
            _mutex.unlock();
        }
        void clear() {
            _mutex.lock();
            __old__staticmap<K,V>::_size = 0;
            _mutex.unlock();
        }
        udword_t count(K key) {
            _mutex.lock();
            udword_t c = __old__staticmap<K, V>::count(key);
            _mutex.unlock();
            return c;
        }
    protected:
        Mutex _mutex;
    };

    template<typename K, typename V>
    class __old_tsdynamicmap : public __old_dynamicmap<K,V> {
    private:
        Mutex _mutex;
    public:
        class tsCellPosition : public __old_dynamicmap<K,V>::CellPosition {
        public:
            __old_tsdynamicmap * map;
            V operator=(V x) {
                __old_dynamicmap<K,V>::CellPosition::operator=(x);
                map->_mutex.unlock();
                return x;
            }
            operator V () {
                try {
                    V x = __old_dynamicmap<K,V>::CellPosition::operator V();
                    map->_mutex.unlock();
                    return x;
                }
                catch (MapError&) {
                    map->_mutex.unlock();
                    throw;
                }
            }
        };

        __old_tsdynamicmap() {
            __old_dynamicmap<K,V>::_root = NULL;
            __old_dynamicmap<K,V>::_size = 0;
        }
        __old_tsdynamicmap(__old_tsdynamicmap & o) {
            __old_dynamicmap<K,V>::_root = new typename __old_dynamicmap<K,V>::Node(*(o._root));
            __old_dynamicmap<K,V>::_size = o._size;
        }
        virtual ~__old_tsdynamicmap() {
            delete __old_dynamicmap<K,V>::_root;
        }
        __old_tsdynamicmap & operator=(__old_tsdynamicmap & o) {
            delete __old_dynamicmap<K,V>::_root;
            __old_dynamicmap<K,V>::_root = new typename __old_dynamicmap<K,V>::Node(*(o._root));
            __old_dynamicmap<K,V>::_size = o._size;
        }
        bool empty() {
            _mutex.lock();
            bool b = __old_dynamicmap<K,V>::_size == 0;
            _mutex.unlock();
            return b;
        }
        udword_t size() {
            _mutex.lock();
            udword_t s = __old_dynamicmap<K,V>::_size;
            _mutex.unlock();
            return s;
        }
        tsCellPosition operator[](K key) {
            _mutex.lock();
            tsCellPosition p;
            typename __old_dynamicmap<K,V>::Node * n = __old_dynamicmap<K,V>::_root, * n_ant = NULL;
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
                __old_dynamicmap<K,V>::erase(key);
                _mutex.unlock();
            }
            catch (MapError &) {
                _mutex.unlock();
                throw;
            }
        }
        void swap(__old_tsdynamicmap & o) {
            _mutex.lock();
            o._mutex.lock();
            typename __old_dynamicmap<K,V>::Node * tmproot = __old_dynamicmap<K,V>::_root;
            udword_t tmpsize = __old_dynamicmap<K,V>::_size;
            __old_dynamicmap<K,V>::_root = o._root;
            __old_dynamicmap<K,V>::_size = o._size;
            o._root = tmproot;
            o._size = tmpsize;
            o._mutex.unlock();
            _mutex.unlock();
        }
        void clear() {
            _mutex.lock();
            __old_dynamicmap<K,V>::clear();
            _mutex.unlock();
        }
        udword_t count(K key) {
            _mutex.lock();
            udword_t c = __old_dynamicmap<K,V>::count(key);
            _mutex.unlock();
            return c;
        }
    };

}

#ifdef _MSC_VER
    #pragma warning(disable: 4521 4522)
#endif

#endif //__TORUS_MAP_H
