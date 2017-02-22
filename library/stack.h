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

#define _TTL_STACK_DEFAULT_SIZE 10

namespace ttl {
    template<typename T>
    class stack {
    public:
        virtual void push(T t) = 0;
        virtual void pop() = 0;
        virtual T top() = 0;
        virtual void clear() {
            while(!empty()) {
                pop();
            }
        }
        virtual bool empty() = 0;
        virtual t_udword size() = 0;
        virtual ~stack() = 0;
    };

    template<typename T>
    class fixedstack : public stack<T> {
    public:
        fixedstack(t_udword size=_TTL_STACK_DEFAULT_SIZE) {
            _stack = new T[size];
            _top = 0;
            _size = size;
        }

        virtual void push(T t) {
            if (_top == _size) {
                throw StackError("stack is full.");
            }
            _stack[_top++] = t;
        }

        virtual void pop() {
            if (!_top) {
                throw StackError("stack is empty.");
            }
            _top--;
        }

        virtual T top() {
            if (!_top) {
                throw StackError("stack is empty.");
            }
            return _stack[_top - 1];
        }

        virtual bool empty() {
            return _top == 0;
        }

        virtual t_udword size() {
            return _top;
        }

        virtual ~fixedstack() {
            delete _stack;
        }

    private:
        T * _stack;
        t_udword _top, _size;
    };

    template <typename T>
    class fixedgrowingstack : public stack<T> {
    public:
        fixedgrowingstack(t_udword size=_TTL_STACK_DEFAULT_SIZE) {
            _stack = new T[size];
            _top = 0;
            _size = size;
        }

        virtual void push(T t) {
            if (_top == _size) {
                T * nstack = new T[_size + _TTL_STACK_DEFAULT_SIZE];
                memcpy(nstack, _stack, sizeof(T) * _size);
                delete _stack;
                _stack = nstack;
                _size += _TTL_STACK_DEFAULT_SIZE;
            }
            _stack[_top++] = t;
        }

        virtual void pop() {
            if (!_top) {
                throw StackError("stack is empty.");
            }
            _top--;
        }

        virtual T top() {
            if (!_top) {
                throw StackError("stack is empty.");
            }
            return _stack[_top - 1];
        }

        virtual bool empty() {
            return _top == 0;
        }

        virtual t_udword size() {
            return _top;
        }


        virtual ~fixedgrowingstack() {
            delete _stack;
        }

    private:
        T * _stack;
        t_udword _top, _size;
    };

    template<typename T>
    class dynamicstack : public stack<T> {
    public:
        dynamicstack() {
            _top = NULL;
            _size = 0;
        }

        virtual void push(T t) {
            _dynamicstackitem * ntop = new _dynamicstackitem(t, _top);
            _top = ntop;
            _size++;
        }

        virtual void pop() {
            if (_top == NULL) {
                throw StackError("stack is empty.");
            }
            _dynamicstackitem * oldtop = _top;
            _top = oldtop->_next;
            delete oldtop;
            _size--;
        }

        virtual T top() {
            if (_top == NULL) {
                throw StackError("stack is empty.");
            }
            return _top->_item;
        }

        virtual bool empty() {
            return _top == NULL;
        }

        virtual t_udword size(){
            return _size;
        }

        virtual ~dynamicstack() {
            stack<T>::clear();
        };
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

    template <typename T>
    class tsfixedstack : public fixedstack<T> {
    public:
        tsfixedstack(t_udword size=_TTL_STACK_DEFAULT_SIZE) : fixedstack<T>(size) {}
        virtual void push(T t) {
            _mutex.lock();
            fixedstack<T>::push(t);
            _mutex.unlock();
        }

        virtual void pop() {
            _mutex.lock();
            fixedstack<T>::pop();
            _mutex.unlock();
        }

        virtual T top() {
            _mutex.lock();
            T x = fixedstack<T>::top();
            _mutex.unlock();
            return x;
        }

        virtual void clear() {
            _mutex.lock();
            fixedstack<T>::clear();
            _mutex.unlock();
        }

        virtual bool empty() {
            _mutex.lock();
            bool b = fixedstack<T>::empty();
            _mutex.unlock();
            return b;
        }

        virtual t_udword size() {
            _mutex.lock();
            t_udword s = fixedstack<T>::size();
            _mutex.unlock();
            return s;
        }
    private:
        Mutex _mutex;
    };

    template <typename T>
    class tsfixedgrowingstack : public fixedgrowingstack<T> {
    public:
        tsfixedgrowingstack(t_udword size=_TTL_STACK_DEFAULT_SIZE) : fixedgrowingstack<T>(size) {}
        virtual void push(T t) {
            _mutex.lock();
            fixedgrowingstack<T>::push(t);
            _mutex.unlock();
        }

        virtual void pop() {
            _mutex.lock();
            fixedgrowingstack<T>::pop();
            _mutex.unlock();
        }

        virtual T top() {
            _mutex.lock();
            T x = fixedgrowingstack<T>::top();
            _mutex.unlock();
            return x;
        }

        virtual void clear() {
            _mutex.lock();
            fixedgrowingstack<T>::clear();
            _mutex.unlock();
        }

        virtual bool empty() {
            _mutex.lock();
            bool b = fixedgrowingstack<T>::empty();
            _mutex.unlock();
            return b;
        }

        virtual t_udword size() {
            _mutex.lock();
            t_udword s = fixedgrowingstack<T>::size();
            _mutex.unlock();
            return s;
        }
    private:
        Mutex _mutex;
    };

    template <typename T>
    class tsdynamicstack : public dynamicstack<T> {
    public:
        tsdynamicstack(t_udword size=_TTL_STACK_DEFAULT_SIZE) : dynamicstack<T>() {}
        virtual void push(T t) {
            _mutex.lock();
            dynamicstack<T>::push(t);
            _mutex.unlock();
        }

        virtual void pop() {
            _mutex.lock();
            dynamicstack<T>::pop();
            _mutex.unlock();
        }

        virtual T top() {
            _mutex.lock();
            T x = dynamicstack<T>::top();
            _mutex.unlock();
            return x;
        }

        virtual void clear() {
            _mutex.lock();
            dynamicstack<T>::clear();
            _mutex.unlock();
        }

        virtual bool empty() {
            _mutex.lock();
            bool b = dynamicstack<T>::empty();
            _mutex.unlock();
            return b;
        }

        virtual t_udword size() {
            _mutex.lock();
            t_udword s = dynamicstack<T>::size();
            _mutex.unlock();
            return s;
        }
    private:
        Mutex _mutex;
    };
}

#endif //__TORUS_STACK_H
