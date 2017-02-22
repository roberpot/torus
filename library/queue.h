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

#ifndef __TORUS_QUEUE_H
#define __TORUS_QUEUE_H

#include <cstring>
#include "errors.h"
#include "../threads/mutex.h"

#define _TTL_QUEUE_DEFAULT_SIZE 10

namespace ttl {
    template <typename T>
    class queue {
    public:
        virtual void push(T t) = 0;
        virtual void pop() = 0;
        virtual T front() = 0;
        virtual void clear() {
            while(!empty()) {
                pop();
            }
        }
        virtual bool empty() = 0;
        virtual t_udword size() = 0;
        virtual ~queue() = 0;
    private:
    };

    template <typename T>
    class fixedqueue : public queue<T> {
    public:
        fixedqueue(t_udword size=_TTL_QUEUE_DEFAULT_SIZE) {
            _queue = new T[size];
            _size = size;
            _front = 0;
            _end = 0;
            _empty = true;
        }

        virtual void push(T t) {
            if (_end == _front && !_empty) {
                throw QueueError("queue is full.");
            }
            _queue[_end] = t;
            _end++;
            _end %= _size;
            _empty = false;
        }

        virtual void pop() {
            if (_empty) {
                throw QueueError("queue is empty.");
            }
            _front++;
            _front %= _size;
            if (_front == _end) {
                _empty = true;
            }
        }

        virtual T front() {
            if (_empty) {
                throw QueueError("queue is empty.");
            }
            return _queue[_front];
        }

        virtual bool empty() {
            return _empty;
        }

        virtual t_udword size() {
            return _size;
        }

        virtual ~fixedqueue() {
            delete _queue;
        }

    private:
        T * _queue;
        t_udword _front, _end, _size;
        bool _empty;
    };

    template <typename T>
    class fixedgrowingqueue : public queue<T> {
    public:
        fixedgrowingqueue(t_udword size=_TTL_QUEUE_DEFAULT_SIZE) {
            _queue = new T[size];
            _size = size;
            _front = 0;
            _end = 0;
            _empty = true;
        }

        virtual void push(T t) {
            if (_end == _front && !_empty) {
                T * nqueue = new T[_size + _TTL_QUEUE_DEFAULT_SIZE];
                if (_front == 0) {
                    memcpy(nqueue, _queue, sizeof(T) * _size);
                } else {
                    memcpy(nqueue, &_queue[_front], sizeof(T) * (_size - _front));
                    memcpy(&nqueue[_size - _front], _queue, sizeof(T) * _front);
                    _end = _size - 1;
                    _front = 0;
                }
                delete _queue;
                _queue = nqueue;
                _size += _TTL_QUEUE_DEFAULT_SIZE;
            }
            _queue[_end] = t;
            _end++;
            _end %= _size;
            _empty = false;
        }

        virtual void pop() {
            if (_empty) {
                throw QueueError("queue is empty.");
            }
            _front++;
            _front %= _size;
            if (_front == _end) {
                _empty = true;
            }
        }

        virtual T front() {
            if (_empty) {
                throw QueueError("queue is empty.");
            }
            return _queue[_front];
        }

        virtual bool empty() {
            return _empty;
        }

        virtual t_udword size() {
            return _size;
        }

        virtual ~fixedgrowingqueue() {
            delete _queue;
        }

    private:
        T * _queue;
        t_udword _front, _end, _size;
        bool _empty;
    };

    template <typename T>
    class dynamicqueue : public queue<T> {
    public:
        dynamicqueue() {
            _size = 0;
            _front = NULL;
            _end = NULL;
        }

        virtual void push(T t) {
            _dynamicqueueitem * item = new _dynamicqueueitem(t, NULL);
            _end->_next = item;
            _end = item;
            _size++;
        }
        virtual void pop() = 0;
        virtual T front() = 0;
        virtual void clear() {
            while(!empty()) {
                pop();
            }
        }
        virtual bool empty() = 0;
        virtual t_udword size() = 0;
        virtual ~dynamicqueue() = 0;
    private:
        struct _dynamicqueueitem {
        public:
            _dynamicqueueitem(T item, _dynamicqueueitem * next) {
                _item = item;
                _next = next;
            }
            T _item;
            _dynamicqueueitem * _next;
        };
        _dynamicqueueitem * _front, *_end;
        t_udword _size;
    };

    template <typename T>
    class tsfixedqueue : public fixedqueue<T> {
    public:
        virtual void push(T t) {
            _mutex.lock();
            fixedqueue<T>::push(t);
            _mutex.unlock();
        }

        virtual void pop() {
            _mutex.lock();
            fixedqueue<T>::pop();
            _mutex.unlock();
        }

        virtual T front() {
            _mutex.lock();
            T x = fixedqueue<T>::front();
            _mutex.unlock();
            return x;
        }

        virtual void clear() {
            _mutex.lock();
            fixedqueue<T>::clear();
            _mutex.unlock();
        }

        virtual bool empty() {
            _mutex.lock();
            bool b = fixedqueue<T>::empty();
            _mutex.unlock();
            return b;
        }

        virtual t_udword size() {
            _mutex.lock();
            t_udword s = fixedqueue<T>::size();
            _mutex.unlock();
            return s;
        }
    private:
        Mutex _mutex;
    };

    template <typename T>
    class tsfixedgrowingqueue : public fixedgrowingqueue<T> {
    public:
        virtual void push(T t) {
            _mutex.lock();
            fixedgrowingqueue<T>::push(t);
            _mutex.unlock();
        }

        virtual void pop() {
            _mutex.unlock();
            fixedgrowingqueue<T>::pop();
            _mutex.lock();
        }

        virtual T front() {
            _mutex.unlock();
            T x = fixedgrowingqueue<T>::front();
            _mutex.lock();
            return x;
        }

        virtual void clear() {
            _mutex.unlock();
            fixedgrowingqueue<T>::clear();
            _mutex.lock();
        }

        virtual bool empty() {
            _mutex.unlock();
            bool b = fixedgrowingqueue<T>::empty();
            _mutex.lock();
            return b;
        }

        virtual t_udword size() {
            _mutex.unlock();
            t_udword s = fixedgrowingqueue<T>::size();
            _mutex.lock();
            return s;
        }
    private:
        Mutex _mutex;
    };

    template <typename T>
    class tsdynamicqueue : public dynamicqueue<T> {
    public:
        virtual void push(T t) {
            _mutex.lock();
            dynamicqueue<T>::push(t);
            _mutex.unlock();
        }

        virtual void pop() {
            _mutex.lock();
            dynamicqueue<T>::pop();
            _mutex.unlock();
        }

        virtual T front() {
            _mutex.lock();
            T x = dynamicqueue<T>::front();
            _mutex.unlock();
            return x;
        }

        virtual void clear() {
            _mutex.lock();
            dynamicqueue<T>::clear();
            _mutex.unlock();
        }

        virtual bool empty() {
            _mutex.lock();
            bool b = dynamicqueue<T>::empty();
            _mutex.unlock();
            return b;
        }

        virtual t_udword size() {
            _mutex.lock();
            t_udword s = dynamicqueue<T>::size();
            _mutex.unlock();
            return s;
        }
    private:
        Mutex _mutex;
    };
}

#endif //__TORUS_QUEUE_H
