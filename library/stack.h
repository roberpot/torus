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

#ifndef __TORUS_STACK_H
#define __TORUS_STACK_H

#include <cstring>

#include "types.h"
#include "../threads/mutex.h"
#include "errors.h"
#include "../debug/info.h"

#define _TTL_STACK_DEFAULT_SIZE 10

namespace ttl {
    template<typename T>
    class fixedstack {
    public:
        fixedstack(t_udword size=_TTL_STACK_DEFAULT_SIZE) {
            _stack = new T[size];
            _top = 0;
            _size = size;
        }

        fixedstack(fixedstack<T> & o) {
            _stack = new T[o._size];
            _top = o._top;
            _size = o._size;
            for(t_udword i = 0; i < _top; ++i) _stack[i] = o._stack[i];
        }

        fixedstack(const fixedstack<T> & o) {
            _stack = new T[o._size];
            _top = o._top;
            _size = o._size;
            for(t_udword i = 0; i < _top; ++i) _stack[i] = o._stack[i];
        }

        ~fixedstack() {
            delete[] _stack;
        }

        fixedstack & operator=(fixedstack<T> & o) {
            delete[] _stack;
            _stack = new T[o._size];
            _top = o._top;
            _size = o._size;
            for(t_udword i = 0; i < _top; ++i) _stack[i] = o._stack[i];
            return *this;
        }

        fixedstack & operator=(const fixedstack<T> & o) {
            delete[] _stack;
            _stack = new T[o._size];
            _top = o._top;
            _size = o._size;
            for(t_udword i = 0; i < _top; ++i) _stack[i] = o._stack[i];
            return *this;
        }
        
        void push(T t) {
            if (_top == _size) {
                throw StackError("stack is full.");
            }
            _stack[_top++] = t;
        }

        void pop() {
            if (!_top) {
                throw StackError("stack is empty.");
            }
            _top--;
        }

        T top() {
            if (!_top) {
                throw StackError("stack is empty.");
            }
            return _stack[_top - 1];
        }

        void clear() {
            while(!empty()) {
                pop();
            }
        }

        bool empty() {
            return _top == 0;
        }

        t_udword size() {
            return _top;
        }

    private:
        T * _stack;
        t_udword _top, _size;
    };

    template <typename T>
    class fixedgrowingstack {
    public:
        fixedgrowingstack(t_udword size=_TTL_STACK_DEFAULT_SIZE) {
            _stack = new T[size];
            _top = 0;
            _size = size;
        }

        fixedgrowingstack(fixedgrowingstack<T> & o) {
            _stack = new T[o._size];
            _top = o._top;
            _size = o._size;
            for(t_udword i = 0; i < _top; ++i) _stack[i] = o._stack[i];
        }

        fixedgrowingstack(const fixedgrowingstack<T> & o) {
            _stack = new T[o._size];
            _top = o._top;
            _size = o._size;
            for(t_udword i = 0; i < _top; ++i) _stack[i] = o._stack[i];
        }

        ~fixedgrowingstack() {
            delete[] _stack;
        }

        fixedgrowingstack & operator=(fixedgrowingstack<T> & o) {
            _stack = new T[o._size];
            _top = o._top;
            _size = o._size;
            for(t_udword i = 0; i < _top; ++i) _stack[i] = o._stack[i];
            return *this;
        }

        fixedgrowingstack & operator=(const fixedgrowingstack<T> & o) {
            _stack = new T[o._size];
            _top = o._top;
            _size = o._size;
            for(t_udword i = 0; i < _top; ++i) _stack[i] = o._stack[i];
            return *this;
        }

        void push(T t) {
            if (_top == _size) {
                T * nstack = new T[_size + _TTL_STACK_DEFAULT_SIZE];
                for(t_udword i = 0; i < _top; ++i) nstack[i] = _stack[i];
                delete[] _stack;
                _stack = nstack;
                _size += _TTL_STACK_DEFAULT_SIZE;
            }
            _stack[_top++] = t;
        }

        void pop() {
            if (!_top) {
                throw StackError("stack is empty.");
            }
            _top--;
        }

        T top() {
            if (!_top) {
                throw StackError("stack is empty.");
            }
            return _stack[_top - 1];
        }

        void clear() {
            while(!empty()) {
                pop();
            }
        }

        bool empty() {
            return _top == 0;
        }

        t_udword size() {
            return _top;
        }

    private:
        T * _stack;
        t_udword _top, _size;
    };

    template<typename T>
    class dynamicstack {
    public:
        dynamicstack() {
            _top = NULL;
            _size = 0;
        }

        // To allow thread secure wrapper constructor.
        dynamicstack(t_udword _) : dynamicstack() { UNREFERENCED_PARAMETER(_); }

        dynamicstack(dynamicstack<T> & o) {
            _size = o._size;
            _top = NULL;
            _dynamicstackitem * next = o._top, * prev;
            if (next) {
                _top = new _dynamicstackitem(next->_item, NULL);
                prev = _top;
                next = next->_next;
                while (next) {
                    prev->_next = new _dynamicstackitem(next->_item, NULL);
                    prev = prev->_next;
                    next = next->_next;
                }
            }
        }

        dynamicstack(const dynamicstack<T> & o) {
            _size = o._size;
            _top = NULL;
            _dynamicstackitem * next = o._top, * prev;
            if (next) {
                _top = new _dynamicstackitem(next->_item, NULL);
                prev = _top;
                next = next->_next;
                while (next) {
                    prev->_next = new _dynamicstackitem(next->_item, NULL);
                    prev = prev->_next;
                    next = next->_next;
                }
            }
        }

        ~dynamicstack() {
            clear();
        };

        dynamicstack & operator=(dynamicstack<T> & o) {
            _size = o._size;
            _top = NULL;
            _dynamicstackitem * next = o._top, * prev;
            if (next) {
                _top = new _dynamicstackitem(next->_item, NULL);
                prev = _top;
                next = next->_next;
                while (next) {
                    prev->_next = new _dynamicstackitem(next->_item, NULL);
                    prev = prev->_next;
                    next = next->_next;
                }
            }
            return *this;
        }

        dynamicstack & operator=(const dynamicstack<T> & o) {
            _size = o._size;
            _top = NULL;
            _dynamicstackitem * next = o._top, * prev;
            if (next) {
                _top = new _dynamicstackitem(next->_item, NULL);
                prev = _top;
                next = next->_next;
                while (next) {
                    prev->_next = new _dynamicstackitem(next->_item, NULL);
                    prev = prev->_next;
                    next = next->_next;
                }
            }
            return *this;
        }

        void push(T t) {
            _dynamicstackitem * ntop = new _dynamicstackitem(t, _top);
            _top = ntop;
            _size++;
        }

        void pop() {
            if (_top == NULL) {
                throw StackError("stack is empty.");
            }
            _dynamicstackitem * oldtop = _top;
            _top = oldtop->_next;
            delete oldtop;
            _size--;
        }

        T top() {
            if (_top == NULL) {
                throw StackError("stack is empty.");
            }
            return _top->_item;
        }

        void clear() {
            while(!empty()) {
                pop();
            }
        }

        bool empty() {
            return _top == NULL;
        }

        t_udword size(){
            return _size;
        }

    private:
        struct _dynamicstackitem {
        public:
            _dynamicstackitem(T item, _dynamicstackitem * next) {
                _item = item;
                _next = next;
            }
            T _item;
            _dynamicstackitem * _next;
        };
        _dynamicstackitem * _top;
        t_udword _size;
    };

    template <typename T, class S>
    class tsstack {
    public:
        tsstack(t_udword size=_TTL_STACK_DEFAULT_SIZE) : _s(size) {}

        tsstack(tsstack<T, S> & o) {
            o._mutex.lock();
            _s = o._s;
            o._mutex.unlock();
        }

        tsstack(const tsstack<T, S> & o) {
            o._mutex.lock();
            _s = o._s;
            o._mutex.unlock();
        }

        tsstack & operator=(tsstack<T, S> & o) {
            _mutex.lock();
            o._mutex.lock();
            _s = o._s;
            o._mutex.unlock();
            _mutex.unlock();
        }

        tsstack & operator=(const tsstack<T, S> & o) {
            _mutex.lock();
            o._mutex.lock();
            _s = o._s;
            o._mutex.unlock();
            _mutex.unlock();
        }

        void push(T t) {
            _mutex.lock();
            _s.push(t);
            _mutex.unlock();
        }

        void pop() {
            _mutex.lock();
            _s.pop();
            _mutex.unlock();
        }

        T top() {
            _mutex.lock();
            T x = _s.top();
            _mutex.unlock();
            return x;
        }

        void clear() {
            _mutex.lock();
            _s.clear();
            _mutex.unlock();
        }

        bool empty() {
            _mutex.lock();
            bool b = _s.empty();
            _mutex.unlock();
            return b;
        }

        t_udword size() {
            _mutex.lock();
            t_udword s = _s.size();
            _mutex.unlock();
            return s;
        }
    private:
        Mutex _mutex;
        S _s;
    };

    template<typename T>
    using tsfixedstack = tsstack<T, fixedstack<T>>;

    template<typename T>
    using tsfixedgrowingstack = tsstack<T, fixedgrowingstack<T>>;

    template<typename T>
    using tsdynamicstack = tsstack<T, dynamicstack<T>>;
}

#endif //__TORUS_STACK_H
