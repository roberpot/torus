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
#include "errors.h"
#include "types.h"
#include "../threads/mutex.h"

#ifdef _MSC_VER
#pragma warning(disable: 4521 4522)
#endif

#define _TTL_VECTOR_DEFAULT_SIZE 10

namespace ttl {
    template <typename T>
    class vector {
    public:
        vector(t_udword size=_TTL_VECTOR_DEFAULT_SIZE) {
            _capacity = size;
            _vector = new T[_capacity];
            _size = 0;
        }

        vector(t_udword size, T v) : vector(size) {
            for (t_udword i = 0; i < _capacity; ++i) _vector[i] = v;
            _size = _capacity;
        }

        vector(vector<T> & o) : vector(o._capacity) {
            _size = o._size;
            for (t_udword i = 0; i < _size; ++i) _vector[i] = o._vector[i];
        }

        vector(const vector<T> & o) : vector(o._capacity) {
            _size = o._size;
            for (t_udword i = 0; i < _size; ++i) _vector[i] = o._vector[i];
        }

        virtual ~vector() { delete[] _vector; }

        vector & operator=(vector<T> & o) {
            delete[] _vector;
            _capacity = o._capacity;
            _vector = new T[_capacity];
            _size = o._size;
            for (t_udword i = 0; i < _size; ++i) _vector[i] = o._vector[i];
            return *this;
        }

        vector & operator=(const vector<T> & o) {
            delete[] _vector;
            _capacity = o._capacity;
            _vector = new T[_capacity];
            _size = o._size;
            for (t_udword i = 0; i < _size; ++i) _vector[i] = o._vector[i];
            return *this;
        }

        virtual t_udword size() { return _size; }
        virtual t_udword max_size() { return TDWORD_MAX; }
        virtual void resize(t_udword s) {
            T * n = new T[s];
            t_udword nsize = _size;
            if (s < _size) nsize = s;
            for (t_udword i = 0; i < nsize; ++i) n[i] = _vector[i];
            _size = nsize;
            _capacity = s;
            delete[] _vector;
            _vector = n;
        }
        virtual void resize(t_udword s, T v) {
            resize(s);
            for (t_udword i = _size; i < _capacity; ++i) {
                _vector[i] = v;
            }
            _size = _capacity;
        }
        virtual t_udword capacity() { return _capacity; }
        virtual bool empty() { return !_size; }
        virtual void reserve(t_udword n) {
            if (_capacity < n) {
                resize(n + _TTL_VECTOR_DEFAULT_SIZE);
            }
        }
        virtual void shrink_to_fit() {
            if (_size < _capacity) {
                T * n = new T[_size];
                for (t_udword i = 0; i < _size; ++i) n[i] = _vector[i];
                delete[] _vector;
                _vector = n;
                _capacity = _size;
            }
        }
        virtual T & operator[](t_udword n) { return _vector[n]; }
        virtual T & at(t_udword n) {
            if (n >= _size) {
                throw VectorError("Out of range.");
            }
            return _vector[n];
        }
        virtual T & front() { return _vector[0]; }
        virtual T & back() { return _vector[_size - 1]; }
        virtual T * data() { return _vector; }
        virtual void assign(t_udword size, T t) {
            reserve(size);
            for(t_udword i = 0; i < size; ++i) {
                _vector[i] = t;
            }
            _size = size;
        }
        virtual void push_back(T t) {
            reserve(_size + 1);
            _vector[_size] = t;
            _size++;
        }
        virtual void pop_back() {
            if (_size) {
                _size--;
            }
        }
        virtual t_udword insert(t_udword p, T t) {
            t_udword new_size = _size + 1;
            if (p >= _size) new_size = p + 1;
            reserve(new_size);
            if (p < _size) {
                memmove(&_vector[p + 1], &_vector[p], sizeof(T) * (_size - p));
            }
            _vector[p] = t;
            _size = new_size;
            return p;
        }
        virtual t_udword insert(t_udword p, t_udword n, T t) {
            t_udword new_size = _size + n;
            if (p >= _size) new_size = p + n;
            reserve(new_size);
            if (p < _size) {
                memmove(&_vector[p + n], &_vector[p], sizeof(T) * (_size - p));
            }
            for(t_udword i = 0; i < n; ++i) _vector[i + p] = t;
            _size = new_size;
            return p;
        }
        virtual t_udword insert(t_udword p, vector & v) {
            t_udword n = v._size;
            t_udword new_size = _size + n;
            if (p >= _size) new_size = p + n;
            reserve(new_size);
            if (p < _size) {
                memmove(&_vector[p + n], &_vector[p], sizeof(T) * (_size - p));
            }
            for(t_udword i = 0; i < n; ++i) _vector[i + p] = v._vector[i];
            _size = new_size;
            return p;
        }
        virtual t_udword erase(t_udword p) {
            if (p >= _size) throw VectorError("Out of range.");
            if (p < _size - 1) {
                memmove(&_vector[p], &_vector[p + 1], _size - p - 1);
            }
            _size--;
            return p;
        }
        virtual t_udword erase(t_udword init, t_udword end) {
            if (init >= _size) throw VectorError("Out of range.");
            if (end >= _size) throw VectorError("Out of range.");
            if (end < _size - 1) {
                memmove(&_vector[init], &_vector[end + 1], _size - end - 1);
            }
            _size -= (end - init + 1);
            return init;
        }
        virtual void swap(vector & v) {
            T * tmp_vector = _vector;
            t_udword tmp_size = _size;
            t_udword tmp_capacity = _capacity;
            _vector = v._vector;
            _size = v._size;
            _capacity = v._capacity;
            v._vector = tmp_vector;
            v._size = tmp_size;
            v._capacity = tmp_capacity;
        }
        virtual void clear() {
            _size = 0;
        }
        bool operator==(vector<T> o) {
            if (_size != o._size) { return false; }
            for(t_udword i = 0; i < _size; ++i) {
                if (_vector[i] != o._vector[i]) { return false; }
            }
            return true;
        }
        bool operator!=(vector<T> o) {
            if (_size != o._size) { return true; }
            for(t_udword i = 0; i < _size; ++i) {
                if (_vector[i] != o._vector[i]) { return true; }
            }
            return false;
        }
        bool operator<(vector<T> o) {
            t_udword size = _size;
            if (o._size < size) { size = o._size; }
            for(t_udword i = 0; i < size; ++i) {
                if (_vector[i] >= o._vector[i]) { return false; }
            }
            return true;
        }
        bool operator<=(vector<T> o) {
            t_udword size = _size;
            if (o._size < size) { size = o._size; }
            for(t_udword i = 0; i < size; ++i) {
                if (_vector[i] > o._vector[i]) { return false; }
            }
            return true;
        }
        bool operator>(vector<T> o) {
            t_udword size = _size;
            if (o._size < size) { size = o._size; }
            for(t_udword i = 0; i < size; ++i) {
                if (_vector[i] <= o._vector[i]) { return false; }
            }
            return true;
        }
        bool operator>=(vector<T> o) {
            t_udword size = _size;
            if (o._size < size) { size = o._size; }
            for(t_udword i = 0; i < size; ++i) {
                if (_vector[i] < o._vector[i]) { return false; }
            }
            return true;
        }
    protected:
        T * _vector;
        t_udword _capacity, _size;
    };

    template <typename T>
    class tsvector {
    public:
        tsvector(t_udword size=_TTL_VECTOR_DEFAULT_SIZE) : _v(size) {}

        tsvector(t_udword size, T v) : _v(size, v) {}

        tsvector(tsvector<T> & o) {
            o._mutex.lock();
            _v = o._v;
            o._mutex.unlock();
        }

        tsvector(const tsvector<T> & o) {
            o._mutex.lock();
            _v = o._v;
            o._mutex.unlock();
        }

        virtual ~tsvector() {}

        tsvector & operator=(tsvector & o) {
            _mutex.lock();
            o._mutex.lock();
            _v = o._v;
            o._mutex.unlock();
            _mutex.unlock();
            return *this;
        }

        tsvector & operator=(const tsvector & o) {
            _mutex.lock();
            o._mutex.lock();
            _v = o._v;
            o._mutex.unlock();
            _mutex.unlock();
            return *this;
        }

        t_udword size() {
            _mutex.lock();
            t_udword s = _v.size();
            _mutex.unlock();
            return s;
        }
        void resize(t_udword s) {
            _mutex.lock();
            _v.resize(s);
            _mutex.unlock();
        }
        void resize(t_udword s, T v) {
            _mutex.lock();
            _v.resize(s, v);
            _mutex.unlock();
        }
        t_udword capacity() {
            _mutex.lock();
            t_udword c = _v.capacity();
            _mutex.unlock();
            return c;
        }
        bool empty() {
            _mutex.lock();
            bool e = !_v.empty();
            _mutex.unlock();
            return e;
        }
        void reserve(t_udword n) {
            _mutex.unlock();
            _v.reserve(n);
            _mutex.unlock();
        }
        void shrink_to_fit() {
            _mutex.lock();
            _v.shrink_to_fit();
            _mutex.unlock();
        }
        T & operator[](t_udword n) {
            _mutex.lock();
            T t = _v[n];
            _mutex.unlock();
            return t;
        }
        T & at(t_udword n) {
            _mutex.lock();
            T t = _v.at(n);
            _mutex.unlock();
            return t;
        }
        T & front() {
            _mutex.lock();
            T t = _v[0];
            _mutex.unlock();
            return t;
        }
        T & back() {
            _mutex.lock();
            T t = _v[_v.size() - 1];
            _mutex.unlock();
            return t;
        }
        void assign(t_udword size, T t) {
            _mutex.lock();
            _v.assign(size, t);
            _mutex.unlock();
        }
        void push_back(T t) {
            _mutex.lock();
            _v.push_back(t);
            _mutex.unlock();
        }
        void pop_back() {
            _mutex.lock();
            _v.pop_back();
            _mutex.unlock();
        }
        t_udword insert(t_udword p, T t) {
            _mutex.lock();
            t_udword r = _v.insert(p, t);
            _mutex.unlock();
            return r;
        }
        t_udword insert(t_udword p, t_udword n, T t) {
            _mutex.lock();
            t_udword r = _v.insert(p, n, t);
            _mutex.unlock();
            return r;
        }
        t_udword insert(t_udword p, tsvector & v) {
            _mutex.lock();
            v._mutex.lock();
            t_udword r = _v.insert(p, v._v);
            v._mutex.unlock();
            _mutex.unlock();
            return r;
        }
        t_udword erase(t_udword p) {
            _mutex.lock();
            t_udword r = _v.erase(p);
            _mutex.unlock();
            return r;
        }
        t_udword erase(t_udword init, t_udword end) {
            _mutex.lock();
            t_udword r = _v.erase(init, end);
            _mutex.unlock();
            return r;
        }
        void swap(tsvector & v) {
            _mutex.lock();
            v._mutex.lock();
            _v.swap(v._v);
            v._mutex.unlock();
            _mutex.unlock();
        }
        void clear() {
            _mutex.lock();
            _v.clear();
            _mutex.unlock();
        }
        bool operator==(tsvector<T> o) {
            _mutex.lock();
            o._mutex.lock();
            bool b = _v == o._v;
            o._mutex.unlock();
            _mutex.unlock();
            return b;
        }
        bool operator!=(tsvector<T> o) {
            _mutex.lock();
            o._mutex.lock();
            bool b = _v != o._v;
            o._mutex.unlock();
            _mutex.unlock();
            return b;
        }
        bool operator<(tsvector<T> o) {
            _mutex.lock();
            o._mutex.lock();
            bool b = _v < o._v;
            o._mutex.unlock();
            _mutex.unlock();
            return b;
        }
        bool operator<=(tsvector<T> o) {
            _mutex.lock();
            o._mutex.lock();
            bool b = _v <= o._v;
            o._mutex.unlock();
            _mutex.unlock();
            return b;
        }
        bool operator>(tsvector<T> o) {
            _mutex.lock();
            o._mutex.lock();
            bool b = _v > o._v;
            o._mutex.unlock();
            _mutex.unlock();
            return b;
        }
        bool operator>=(tsvector<T> o) {
            _mutex.lock();
            o._mutex.lock();
            bool b = _v >= o._v;
            o._mutex.unlock();
            _mutex.unlock();
            return b;
        }
    private:
        Mutex _mutex;
        vector<T> _v;
    };
}

#include <vector>

#ifdef _MSC_VER
#pragma warning(default: 4521 4522)
#endif
#endif //__TORUS_VECTOR_H
