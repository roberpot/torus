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

#ifndef __TORUS_VECTOR_H
#define __TORUS_VECTOR_H

#include <cstring>

#include <iostream>
#include <memory>

#include <library/errors.h>
#include <library/memory.h>
#include <library/types.h>
#include <threads/mutex.h>

#ifdef _MSC_VER
#pragma warning(disable: 4521 4522)
#endif

#define _TTL_VECTOR_DEFAULT_SIZE 1

namespace ttl {
    template <class T, class Allocator = std::allocator<T>>
    class vector {
    public:
        // Constructors
        /**
         * @brief
         * @param size
         */
        vector(udword_t size = _TTL_VECTOR_DEFAULT_SIZE);
        vector(udword_t size, const T& v);
        vector(const vector& o);
        vector(vector&& o);
        ~vector();
        vector& operator=(const vector& o);
        vector& operator=(vector&& o);
        void assign(udword_t size, const T& t);

        // Element access
        T& at(udword_t n);
        T& operator[](udword_t n);
        T& front();
        T& back();
        T* data();

        // Capacity
        bool empty() const;
        udword_t size() const;
        udword_t max_size() const;
        void reserve(udword_t n);
        udword_t capacity() const;
        void shrink_to_fit();

        // Modifiers
        void clear();
        udword_t insert(udword_t p, const T& t);
        udword_t insert(udword_t p, T&& t);
        udword_t insert(udword_t p, udword_t n, const T& t);
        udword_t insert(udword_t p, const vector& v);
        udword_t erase(udword_t p);
        udword_t erase(udword_t init, udword_t end);
        void push_back(const T& t);
        void push_back(T&& t);
        void pop_back();
        void resize(udword_t s);
        void resize(udword_t s, const T& v);
        void swap(vector & v);

        // Operators.
        bool operator==(const vector& o) const;
        bool operator!=(const vector& o) const;
        bool operator<(const vector& o) const;
        bool operator<=(const vector& o) const;
        bool operator>(const vector& o) const;
        bool operator>=(const vector& o) const;
    protected:
        T* _vector;
        udword_t _capacity;
        udword_t _size;
        Allocator _allocator;
    };

    template <typename T, class V>
    class __T_tsvector {
    public:
        __T_tsvector(udword_t size=_TTL_VECTOR_DEFAULT_SIZE);
        __T_tsvector(udword_t size, const T& v);
        __T_tsvector(const __T_tsvector& o);
        __T_tsvector(__T_tsvector&& o);
        ~__T_tsvector();
        __T_tsvector& operator=(const __T_tsvector& o);
        __T_tsvector& operator=(__T_tsvector&& o);
        void assign(udword_t size, const T& t);

        // Element access
        T at(udword_t n);
        T operator[](udword_t n);
        T front();
        T back();

        // Capacity
        bool empty() const;
        udword_t size() const;
        udword_t max_size() const;
        void reserve(udword_t n);
        udword_t capacity() const;
        void shrink_to_fit();

        // Modifiers
        void clear();
        udword_t insert(udword_t p, const T& t);
        udword_t insert(udword_t p, T&& t);
        udword_t insert(udword_t p, udword_t n, const T& t);
        udword_t insert(udword_t p, const __T_tsvector& v);
        udword_t erase(udword_t p);
        udword_t erase(udword_t init, udword_t end);
        void push_back(const T& t);
        void push_back(T&& t);
        void pop_back();
        void resize(udword_t s);
        void resize(udword_t s, const T& v);
        void swap(__T_tsvector& v);

        // Operators.
        bool operator==(const __T_tsvector& o) const;
        bool operator!=(const __T_tsvector& o) const;
        bool operator<(const __T_tsvector& o) const;
        bool operator<=(const __T_tsvector& o) const;
        bool operator>(const __T_tsvector& o) const;
        bool operator>=(const __T_tsvector& o) const;
    private:
        mutable Mutex _mutex;
        V _v;
    };


    template<typename T, class Allocator = std::allocator<T>>
    using tsvector = __T_tsvector<T, vector<T, Allocator>>;

    /**
     * Implementation.
     */

    template <class T, class Allocator>
    vector<T, Allocator>::vector(udword_t size) {
        _capacity = size;
        _vector = _allocator.allocate(_capacity);
        _size = 0;
    }

    template <class T, class Allocator>
    vector<T, Allocator>::vector(udword_t size, const T & v) {
        _capacity = size;
        _vector = _allocator.allocate(_capacity);
        _size = _capacity;
        for (udword_t i = 0; i < _capacity; ++i) {
            _allocator.construct(&(_vector[i]), v);
        }
    }

    template <class T, class Allocator>
    vector<T, Allocator>::vector(const vector& o) {
        _capacity = o._capacity;
        _vector = _allocator.allocate(_capacity);
        _size = o._size;
        for (udword_t i = 0; i < _size; ++i) {
            _allocator.construct(&(_vector[i]), o._vector[i]);
        }
    }

    template <class T, class Allocator>
    vector<T, Allocator>::vector(vector&& o) {
        _capacity = o._capacity;
        _vector = o._vector;
        _size = o._size;
        o._capacity = 0;
        o._vector = nullptr;
        o._size = 0;
    }

    template <class T, class Allocator>
    vector<T, Allocator>::~vector() {
        if (nullptr != _vector) {
            for (udword_t i = 0; i < _size; ++i) {
                _allocator.destroy(&(_vector[i]));
            }
            _allocator.deallocate(_vector, _capacity);
        }
    }

    template <class T, class Allocator>
    vector<T, Allocator> & vector<T, Allocator>::operator=(const vector& o) {
        if (this != &o) {
            clear();
            _allocator.deallocate(_vector, _capacity);
            _capacity = o._capacity;
            _vector = _allocator.allocate(_capacity);
            _size = o._size;
            for (udword_t i = 0; i < _size; ++i) {
                _allocator.construct(&(_vector[i]), o._vector[i]);
            }
        }
        return *this;
    }

    template <class T, class Allocator>
    vector<T, Allocator> & vector<T, Allocator>::operator=(vector&& o) {
        if (this != &o) {
            swap(o);
        }
        return *this;
    }

    template <class T, class Allocator>
    void vector<T, Allocator>::assign(udword_t size, const T & t) {
        reserve(size);
        for (udword_t i = 0; i < _size; ++i) {
            _allocator.destroy(&(_vector[i]));
        }
        for(udword_t i = 0; i < size; ++i) {
            _allocator.construct(&(_vector[i]), t);
        }
        _size = size;
    }

    template <class T, class Allocator>
    T & vector<T, Allocator>::at(udword_t n) {
        if (n >= _size) {
            throw VectorError("Out of range.");
        }
        return _vector[n];
    }

    template <class T, class Allocator>
    T & vector<T, Allocator>::operator[](udword_t n) {
        return _vector[n];
    }

    template <class T, class Allocator>
    T & vector<T, Allocator>::front() {
        return _vector[0];
    }

    template <class T, class Allocator>
    T & vector<T, Allocator>::back() {
        return _vector[_size - 1];
    }

    template <class T, class Allocator>
    T * vector<T, Allocator>::data() {
        return _vector;
    }

    template <class T, class Allocator>
    bool vector<T, Allocator>::empty() const {
        return _size == 0;
    }

    template <class T, class Allocator>
    udword_t vector<T, Allocator>::size() const {
        return _size;
    }

    template <class T, class Allocator>
    udword_t vector<T, Allocator>::max_size() const {
        return TDWORD_MAX;
    }

    template <class T, class Allocator>
    void vector<T, Allocator>::reserve(udword_t n) {
        if (_capacity < n) {
            udword_t new_capacity = 1;
            while (new_capacity < n) {
                new_capacity = new_capacity << 1;
            }
            T* aux = _allocator.allocate(new_capacity);
            ttl::memory::memmove<T, Allocator>(aux, _vector, _size);
            _allocator.deallocate(_vector, _capacity);
            _vector = aux;
            _capacity = new_capacity;
        }
    }

    template <class T, class Allocator>
    udword_t vector<T, Allocator>::capacity() const {
        return _capacity;
    }

    template <class T, class Allocator>
    void vector<T, Allocator>::shrink_to_fit() {
        if (_size < _capacity) {
            T * n = _allocator.allocate(_size);
            ttl::memory::memmove<T, Allocator>(n, _vector, _size);
            _allocator.deallocate(_vector, _capacity);
            _vector = n;
            _capacity = _size;
        }
    }

    template <class T, class Allocator>
    void vector<T, Allocator>::clear() {
        for(udword_t i = 0; i < _size; ++i) {
            _allocator.destroy(&(_vector[i]));
        }
        _size = 0;
    }

    template <class T, class Allocator>
    udword_t vector<T, Allocator>::insert(udword_t p, const T& t) {
        udword_t new_size = _size + 1;
        if (p >= _size) {
            new_size = p + 1;
        }
        reserve(new_size);
        if (p < _size) {
            udword_t length = _size - p;
            T * aux = _allocator.allocate(length);
            ttl::memory::memmove<T, Allocator>(aux, &(_vector[p]), sizeof(T) * length);
            ttl::memory::memmove<T, Allocator>(&(_vector[p + 1]), aux, sizeof(T) * length);
            _allocator.deallocate(aux, length);
        }
        _allocator.construct(&(_vector[p]), t);
        _size = new_size;
        return p;
    }

    template <class T, class Allocator>
    udword_t vector<T, Allocator>::insert(udword_t p, T&& t) {
        udword_t new_size = _size + 1;
        if (p >= _size) {
            new_size = p + 1;
        }
        reserve(new_size);
        if (p < _size) {
            udword_t length = _size - p;
            T * aux = _allocator.allocate(length);
            ttl::memory::memmove<T, Allocator>(aux, &(_vector[p]), sizeof(T) * length);
            ttl::memory::memmove<T, Allocator>(&(_vector[p + 1]), aux, sizeof(T) * length);
            _allocator.deallocate(aux, length);
        }
        _allocator.construct(&(_vector[p]), t);
        _size = new_size;
        return p;
    }

    template <class T, class Allocator>
    udword_t vector<T, Allocator>::insert(udword_t p, udword_t n, const T& t) {
        udword_t new_size = _size + n;
        if (p >= _size) {
            new_size = p + n;
        }
        reserve(new_size);
        if (p < _size) {
            udword_t length = _size - p;
            T * aux = _allocator.allocate(length);
            ttl::memory::memmove<T, Allocator>(aux, &(_vector[p]), sizeof(T) * length);
            ttl::memory::memmove<T, Allocator>(&(_vector[p + n]), aux, sizeof(T) * length);
            _allocator.deallocate(aux, length);
        }
        for(udword_t i = 0; i < n; ++i) {
            _allocator.construct(&(_vector[i + p]), t);
        }
        _size = new_size;
        return p;
    }

    template <class T, class Allocator>
    udword_t vector<T, Allocator>::insert(udword_t p, const vector& v) {
        udword_t n = v._size;
        udword_t new_size = _size + n;
        if (p >= _size) {
            new_size = p + n;
        }
        reserve(new_size);
        if (p < _size) {
            udword_t length = _size - p;
            T * aux = _allocator.allocate(length);
            ttl::memory::memmove<T, Allocator>(aux, &(_vector[p]), sizeof(T) * length);
            ttl::memory::memmove<T, Allocator>(&(_vector[p + n]), aux, sizeof(T) * length);
            _allocator.deallocate(aux, length);
        }
        for(udword_t i = 0; i < n; ++i) {
            _allocator.construct(&(_vector[i + p]), v._vector[i]);
        }
        _size = new_size;
        return p;
    }

    template <class T, class Allocator>
    udword_t vector<T, Allocator>::erase(udword_t p) {
        if (p >= _size) {
            throw VectorError("Out of range.");
        }
        _allocator.destroy(&(_vector));
        if (p < _size - 1) {
            udword_t length = _size - p - 1;
            T * aux = _allocator.allocate(length);
            ttl::memory::memmove<T, Allocator>(aux, &(_vector[p + 1]), sizeof(T) * length);
            ttl::memory::memmove<T, Allocator>(&(_vector[p]), aux, sizeof(T) * length);
            _allocator.deallocate(aux, length);
        }
        _size--;
        return p;
    }

    template <class T, class Allocator>
    udword_t vector<T, Allocator>::erase(udword_t init, udword_t end) {
        if (init >= _size) {
            throw VectorError("Out of range.");
        }
        if (end >= _size) {
            throw VectorError("Out of range.");
        }
        if (end < init) {
            throw VectorError("Invalid range.");
        }
        if (end < (_size - 1)) {
            udword_t length = _size - end - 1;
            T * aux = _allocator.allocate(length);
            ttl::memory::memmove<T, Allocator>(aux, &(_vector[end + 1]), sizeof(T) * length);
            for (udword_t i = init; i <= end; ++i) {
                _allocator.destroy(&(_vector[i]));
            }
            ttl::memory::memmove<T, Allocator>(&(_vector[init]), aux, length);
            _allocator.deallocate(aux, length);
        }
        _size -= (end - init + 1);
        return init;
    }

    template <class T, class Allocator>
    void vector<T, Allocator>::push_back(const T& t) {
        reserve(_size + 1);
        _allocator.construct(&(_vector[_size]), t);
        _size++;
    }

    template <class T, class Allocator>
    void vector<T, Allocator>::push_back(T&& t) {
        reserve(_size + 1);
        _allocator.construct(&(_vector[_size]), t);
        _size++;
    }

    template <class T, class Allocator>
    void vector<T, Allocator>::pop_back() {
        if (_size) {
            _allocator.destroy(&(_vector[_size - 1]));
            _size--;
        }
    }

    template <class T, class Allocator>
    void vector<T, Allocator>::resize(udword_t s) {
        reserve(s);
        if (s < _size) {
            for (udword_t i = s; i < _size; ++i) {
                _allocator.destroy(&(_vector[i]));
            }
        } else if (s > _size) {
            for (udword_t i = _size; i < s; ++i) {
                _allocator.construct(&(_vector[i]));
            }
        }
        _size = s;
    }

    template <class T, class Allocator>
    void vector<T, Allocator>::resize(udword_t s, const T & v) {
        reserve(s);
        if (s < _size) {
            for (udword_t i = s; i < _size; ++i) {
                _allocator.destroy(&(_vector[i]));
            }
        } else if (s > _size) {
            for (udword_t i = _size; i < s; ++i) {
                _allocator.construct(&(_vector[i]), v);
            }
        }
        _size = s;
    }

    template <class T, class Allocator>
    void vector<T, Allocator>::swap(vector & v) {
        if (this != &v) {
            T * aux = _vector;
            udword_t aux_size = _size;
            udword_t aux_capacity = _capacity;
            _vector = v._vector;
            _size = v._size;
            _capacity = v._capacity;
            v._vector = aux;
            v._size = aux_size;
            v._capacity = aux_capacity;
        }
    }

    template <class T, class Allocator>
    bool vector<T, Allocator>::operator==(const vector& o) const {
        if (this == &o) {
            return true;
        }
        if (_size != o._size) { return false; }
        for(udword_t i = 0; i < _size; ++i) {
            if (_vector[i] != o._vector[i]) { return false; }
        }
        return true;
    }

    template <class T, class Allocator>
    bool vector<T, Allocator>::operator!=(const vector& o) const {
        if (this == &o) {
            return false;
        }
        if (_size != o._size) { return true; }
        for(udword_t i = 0; i < _size; ++i) {
            if (_vector[i] != o._vector[i]) { return true; }
        }
        return false;
    }

    template <class T, class Allocator>
    bool vector<T, Allocator>::operator<(const vector& o) const {
        if (this == &o) {
            return false;
        }
        udword_t size = _size;
        if (o._size < size) { size = o._size; }
        for(udword_t i = 0; i < size; ++i) {
            if (_vector[i] >= o._vector[i]) { return false; }
        }
        return true;
    }

    template <class T, class Allocator>
    bool vector<T, Allocator>::operator<=(const vector& o) const {
        if (this == &o) {
            return true;
        }
        udword_t size = _size;
        if (o._size < size) { size = o._size; }
        for(udword_t i = 0; i < size; ++i) {
            if (_vector[i] > o._vector[i]) { return false; }
        }
        return true;
    }

    template <class T, class Allocator>
    bool vector<T, Allocator>::operator>(const vector& o) const {
        if (this == &o) {
            return false;
        }
        udword_t size = _size;
        if (o._size < size) { size = o._size; }
        for(udword_t i = 0; i < size; ++i) {
            if (_vector[i] <= o._vector[i]) { return false; }
        }
        return true;
    }

    template <class T, class Allocator>
    bool vector<T, Allocator>::operator>=(const vector& o) const {
        if (this == &o) {
            return true;
        }
        udword_t size = _size;
        if (o._size < size) { size = o._size; }
        for(udword_t i = 0; i < size; ++i) {
            if (_vector[i] < o._vector[i]) { return false; }
        }
        return true;
    }

    // __T_tsvector implementation.

    template <typename T, class V>
    __T_tsvector<T, V>::__T_tsvector(udword_t size) : _v(size) {

    }

    template <typename T, class V>
    __T_tsvector<T, V>::__T_tsvector(udword_t size, const T& v) : _v(size, v) {

    }

    template <typename T, class V>
    __T_tsvector<T, V>::__T_tsvector(const __T_tsvector& o) {
        o._mutex.lock();
        _v = o._v;
        o._mutex.unlock();
    }

    template <typename T, class V>
    __T_tsvector<T, V>::__T_tsvector(__T_tsvector&& o) {
        o._mutex.lock();
        _v.swap(o._v);
        o._mutex.unlock();
    }

    template <typename T, class V>
    __T_tsvector<T, V>::~__T_tsvector() {
        _mutex.lock();
    }

    template <typename T, class V>
    __T_tsvector<T, V>& __T_tsvector<T, V>::operator=(const __T_tsvector& o) {
        if (this != &o) {
            _mutex.lock();
            o._mutex.lock();
            _v = o._v;
            o._mutex.unlock();
            _mutex.unlock();
        }
        return *this;
    }

    template <typename T, class V>
    __T_tsvector<T, V>& __T_tsvector<T, V>::operator=(__T_tsvector&& o) {
        if (this != &o) {
            _mutex.lock();
            o._mutex.lock();
            _v.swap(o._v);
            o._mutex.unlock();
            _mutex.unlock();
        }
        return *this;
    }

    template <typename T, class V>
    void __T_tsvector<T, V>::assign(udword_t size, const T& t) {
        _mutex.lock();
        _v.assign(size, t);
        _mutex.unlock();
    }

    template <typename T, class V>
    T __T_tsvector<T, V>::at(udword_t n) {
        _mutex.lock();
        T t = _v.at(n);
        _mutex.unlock();
        return t;
    }

    template <typename T, class V>
    T __T_tsvector<T, V>::operator[](udword_t n) {
        _mutex.lock();
        T & t = _v[n];
        _mutex.unlock();
        return t;
    }

    template <typename T, class V>
    T __T_tsvector<T, V>::front() {
        _mutex.lock();
        T t = _v[0];
        _mutex.unlock();
        return t;
    }

    template <typename T, class V>
    T __T_tsvector<T, V>::back() {
        _mutex.lock();
        T t = _v[_v.size() - 1];
        _mutex.unlock();
        return t;
    }

    template <typename T, class V>
    bool __T_tsvector<T, V>::empty() const {
        _mutex.lock();
        bool e = !_v.empty();
        _mutex.unlock();
        return e;
    }

    template <typename T, class V>
    udword_t __T_tsvector<T, V>::size() const {
        _mutex.lock();
        udword_t s = _v.size();
        _mutex.unlock();
        return s;
    }

    template <typename T, class V>
    udword_t __T_tsvector<T, V>::max_size() const {
        _mutex.lock();
        udword_t s = _v.max_size();
        _mutex.unlock();
        return s;
    }

    template <typename T, class V>
    void __T_tsvector<T, V>::reserve(udword_t n) {
        _mutex.unlock();
        _v.reserve(n);
        _mutex.unlock();
    }

    template <typename T, class V>
    udword_t __T_tsvector<T, V>::capacity() const {
        _mutex.lock();
        udword_t c = _v.capacity();
        _mutex.unlock();
        return c;
    }

    template <typename T, class V>
    void __T_tsvector<T, V>::shrink_to_fit() {
        _mutex.lock();
        _v.shrink_to_fit();
        _mutex.unlock();
    }

    template <typename T, class V>
    void __T_tsvector<T, V>::clear() {
        _mutex.lock();
        _v.clear();
        _mutex.unlock();
    }

    template <typename T, class V>
    udword_t __T_tsvector<T, V>::insert(udword_t p, const T& t) {
        _mutex.lock();
        udword_t r = _v.insert(p, t);
        _mutex.unlock();
        return r;
    }

    template <typename T, class V>
    udword_t __T_tsvector<T, V>::insert(udword_t p, T&& t) {
        _mutex.lock();
        udword_t r = _v.insert(p, t);
        _mutex.unlock();
        return r;
    }

    template <typename T, class V>
    udword_t __T_tsvector<T, V>::insert(udword_t p, udword_t n, const T& t) {
        _mutex.lock();
        udword_t r = _v.insert(p, n, t);
        _mutex.unlock();
        return r;
    }

    template <typename T, class V>
    udword_t __T_tsvector<T, V>::insert(udword_t p, const __T_tsvector& v) {
        _mutex.lock();
        v._mutex.lock();
        udword_t r = _v.insert(p, v._v);
        v._mutex.unlock();
        _mutex.unlock();
        return r;
    }

    template <typename T, class V>
    udword_t __T_tsvector<T, V>::erase(udword_t p) {
        _mutex.lock();
        udword_t r = _v.erase(p);
        _mutex.unlock();
        return r;
    }

    template <typename T, class V>
    udword_t __T_tsvector<T, V>::erase(udword_t init, udword_t end) {
        _mutex.lock();
        udword_t r = _v.erase(init, end);
        _mutex.unlock();
        return r;
    }

    template <typename T, class V>
    void __T_tsvector<T, V>::push_back(const T& t) {
        _mutex.lock();
        _v.push_back(t);
        _mutex.unlock();
    }

    template <typename T, class V>
    void __T_tsvector<T, V>::push_back(T&& t) {
        _mutex.lock();
        _v.push_back(t);
        _mutex.unlock();
    }

    template <typename T, class V>
    void __T_tsvector<T, V>::pop_back() {
        _mutex.lock();
        _v.pop_back();
        _mutex.unlock();
    }

    template <typename T, class V>
    void __T_tsvector<T, V>::resize(udword_t s) {
        _mutex.lock();
        _v.resize(s);
        _mutex.unlock();
    }

    template <typename T, class V>
    void __T_tsvector<T, V>::resize(udword_t s, const T& v) {
        _mutex.lock();
        _v.resize(s, v);
        _mutex.unlock();
    }

    template <typename T, class V>
    void __T_tsvector<T, V>::swap(__T_tsvector& v) {
        if (this != &v) {
            _mutex.lock();
            v._mutex.lock();
            _v.swap(v._v);
            v._mutex.unlock();
            _mutex.unlock();
        }
    }

    template <typename T, class V>
    bool __T_tsvector<T, V>::operator==(const __T_tsvector& o) const {
        if (this == &o) {
            return true;
        }
        _mutex.lock();
        o._mutex.lock();
        bool b = _v == o._v;
        o._mutex.unlock();
        _mutex.unlock();
        return b;
    }

    template <typename T, class V>
    bool __T_tsvector<T, V>::operator!=(const __T_tsvector& o) const {
        if (this == &o) {
            return false;
        }
        _mutex.lock();
        o._mutex.lock();
        bool b = _v != o._v;
        o._mutex.unlock();
        _mutex.unlock();
        return b;
    }

    template <typename T, class V>
    bool __T_tsvector<T, V>::operator<(const __T_tsvector& o) const {
        if (this == &o) {
            return false;
        }
        _mutex.lock();
        o._mutex.lock();
        bool b = _v < o._v;
        o._mutex.unlock();
        _mutex.unlock();
        return b;
    }

    template <typename T, class V>
    bool __T_tsvector<T, V>::operator<=(const __T_tsvector& o) const {
        if (this == &o) {
            return true;
        }
        _mutex.lock();
        o._mutex.lock();
        bool b = _v <= o._v;
        o._mutex.unlock();
        _mutex.unlock();
        return b;
    }

    template <typename T, class V>
    bool __T_tsvector<T, V>::operator>(const __T_tsvector& o) const {
        if (this == &o) {
            return false;
        }
        _mutex.lock();
        o._mutex.lock();
        bool b = _v > o._v;
        o._mutex.unlock();
        _mutex.unlock();
        return b;
    }

    template <typename T, class V>
    bool __T_tsvector<T, V>::operator>=(const __T_tsvector& o) const {
        if (this == &o) {
            return true;
        }
        _mutex.lock();
        o._mutex.lock();
        bool b = _v >= o._v;
        o._mutex.unlock();
        _mutex.unlock();
        return b;
    }


}

#ifdef _MSC_VER
#pragma warning(default: 4521 4522)
#endif
#endif //__TORUS_VECTOR_H
