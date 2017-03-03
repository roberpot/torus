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

#include <iostream>
#include <cstring>
#include "errors.h"
#include "../threads/mutex.h"
#include "../debug/info.h"

#define _TTL_QUEUE_DEFAULT_SIZE 10

namespace ttl {
    template <typename T>
    class fixedqueue {
    public:
        fixedqueue(t_udword size=_TTL_QUEUE_DEFAULT_SIZE) {
            _queue = new T[size];
            _size = size;
            _front = 0;
            _end = 0;
            _empty = true;
        }

        fixedqueue(fixedqueue<T> & o) {
            _size = o._size;
            _front = 0;
            _end = (o._end - o._front + o._size) % o._size;
            _empty = o._empty;
            _queue = new T[_size];
            if (!_empty) {
                _queue[0] = o._queue[o._front];
                for(t_udword i = 1; i % _size != _end; ++i) _queue[i] = o._queue[(i + o._front) % _size];
            }
        }

        fixedqueue(const fixedqueue<T> & o) {
            _size = o._size;
            _front = 0;
            _end = (o._end - o._front + o._size) % o._size;
            _empty = o._empty;
            _queue = new T[_size];
            if (!_empty) {
                _queue[0] = o._queue[o._front];
                for(t_udword i = 1; i % _size != _end; ++i) _queue[i] = o._queue[(i + o._front) % _size];
            }
        }

        ~fixedqueue() {
            delete[] _queue;
        }

        fixedqueue & operator=(fixedqueue & o) {
            delete[] _queue;
            _size = o._size;
            _front = 0;
            _end = (o._end - o._front + o._size) % o._size;
            _empty = o._empty;
            _queue = new T[_size];
            if (!_empty) {
                _queue[0] = o._queue[o._front];
                for(t_udword i = 1; i % _size != _end; ++i) _queue[i] = o._queue[(i + o._front) % _size];
            }
            return *this;
        }

        fixedqueue & operator=(const fixedqueue & o) {
            delete[] _queue;
            _size = o._size;
            _front = 0;
            _end = (o._end - o._front + o._size) % o._size;
            _empty = o._empty;
            _queue = new T[_size];
            if (!_empty) {
                _queue[0] = o._queue[o._front];
                for(t_udword i = 1; i % _size != _end; ++i) _queue[i] = o._queue[(i + o._front) % _size];
            }
            return *this;
        }

        void push(T t) {
            if (_end == _front && !_empty) {
                throw QueueError("queue is full.");
            }
            _queue[_end] = t;
            _end++;
            _end %= _size;
            _empty = false;
        }

        void pop() {
            if (_empty) {
                throw QueueError("queue is empty.");
            }
            _front++;
            _front %= _size;
            if (_front == _end) {
                _empty = true;
            }
        }

        T front() {
            if (_empty) {
                throw QueueError("queue is empty.");
            }
            return _queue[_front];
        }

        bool empty() {
            return _empty;
        }

        void clear() {
            while(!empty()) {
                pop();
            }
        }

        t_udword size() {
            return _size;
        }

    private:
        T * _queue;
        t_udword _front, _end, _size;
        bool _empty;
    };

    template <typename T>
    class fixedgrowingqueue {
    public:
        fixedgrowingqueue(t_udword size=_TTL_QUEUE_DEFAULT_SIZE) {
            _queue = new T[size];
            _size = size;
            _front = 0;
            _end = 0;
            _empty = true;
        }

        fixedgrowingqueue(fixedgrowingqueue<T> & o) {
            _size = o._size;
            _front = 0;
            _end = (o._end - o._front + o._size) % o._size;
            _empty = o._empty;
            _queue = new T[_size];
            if (!_empty) {
                _queue[0] = o._queue[o._front];
                for(t_udword i = 1; i % _size != _end; ++i) _queue[i] = o._queue[(i + o._front) % _size];
            }
        }

        fixedgrowingqueue(const fixedgrowingqueue<T> & o) {
            _size = o._size;
            _front = 0;
            _end = (o._end - o._front + o._size) % o._size;
            _empty = o._empty;
            _queue = new T[_size];
            if (!_empty) {
                _queue[0] = o._queue[o._front];
                for(t_udword i = 1; i % _size != _end; ++i) _queue[i] = o._queue[(i + o._front) % _size];
            }
        }

        ~fixedgrowingqueue() {
            delete[] _queue;
        }

        fixedgrowingqueue & operator=(fixedgrowingqueue<T> & o) {
            delete[] _queue;
            _size = o._size;
            _front = 0;
            _end = (o._end - o._front + o._size) % o._size;
            _empty = o._empty;
            _queue = new T[_size];
            if (!_empty) {
                _queue[0] = o._queue[o._front];
                for(t_udword i = 1; i % _size != _end; ++i) _queue[i] = o._queue[(i + o._front) % _size];
            }
            return *this;
        }

        fixedgrowingqueue & operator=(const fixedgrowingqueue<T> & o) {
            delete[] _queue;
            _size = o._size;
            _front = 0;
            _end = (o._end - o._front + o._size) % o._size;
            _empty = o._empty;
            _queue = new T[_size];
            if (!_empty) {
                _queue[0] = o._queue[o._front];
                for(t_udword i = 1; i % _size != _end; ++i) _queue[i] = o._queue[(i + o._front) % _size];
            }
        return *this;
        }

        void push(T t) {
            if (_end == _front && !_empty) {
                T * nqueue = new T[_size + _TTL_QUEUE_DEFAULT_SIZE];
                nqueue[0] = _queue[_front];
                for(t_udword i = 1; i % _size != _end; ++i) nqueue[i] = _queue[(i + _front) % _size];
                _end = _size - 1;
                _front = 0;
                delete[] _queue;
                _queue = nqueue;
                _size += _TTL_QUEUE_DEFAULT_SIZE;
            }
            _queue[_end] = t;
            _end++;
            _end %= _size;
            _empty = false;
        }

        void pop() {
            if (_empty) {
                throw QueueError("queue is empty.");
            }
            _front++;
            _front %= _size;
            if (_front == _end) {
                _empty = true;
            }
        }

        T front() {
            if (_empty) {
                throw QueueError("queue is empty.");
            }
            return _queue[_front];
        }

        bool empty() {
            return _empty;
        }

        void clear() {
            while(!empty()) {
                pop();
            }
        }

        t_udword size() {
            return _size;
        }

    private:
        T * _queue;
        t_udword _front, _end, _size;
        bool _empty;
    };

    template <typename T>
    class dynamicqueue {
    public:
        dynamicqueue() {
            _size = 0;
            _front = NULL;
            _end = NULL;
        }

        // To allow thread secure wrapper constructor.
        dynamicqueue(t_udword _) : dynamicqueue() { UNREFERENCED_PARAMETER(_); }

        dynamicqueue(dynamicqueue<T> & o) {
            _size = o._size;
            _end = _front = NULL;
            _dynamicqueueitem * item = o._front;
            if (_size) {
                _front = new _dynamicqueueitem(item->_item);
                _end = _front;
                while (item->_next != NULL) {
                    item = item->_next;
                    _end->_next = new _dynamicqueueitem(item->_item);
                    _end = _end->_next;
                }
            }
        }

        dynamicqueue(const dynamicqueue<T> & o) {
            _size = o._size;
            _end = _front = NULL;
            _dynamicqueueitem * item = o._front;
            if (_size) {
                _front = new _dynamicqueueitem(item->_item);
                _end = _front;
                while (item->_next != NULL) {
                    item = item->_next;
                    _end->_next = new _dynamicqueueitem(item->_item);
                    _end = _end->_next;
                }
            }
        }

        ~dynamicqueue() {
            clear();
        }

        dynamicqueue & operator=(dynamicqueue<T> & o) {
            clear();
            _size = o._size;
            _end = _front = NULL;
            _dynamicqueueitem * item = o._front;
            if (_size) {
                _front = new _dynamicqueueitem(item->_item);
                _end = _front;
                while (item->_next != NULL) {
                    item = item->_next;
                    _end->_next = new _dynamicqueueitem(item->_item);
                    _end = _end->_next;
                }
            }
        }

        dynamicqueue & operator=(const dynamicqueue<T> & o) {
            clear();
            _size = o._size;
            _end = _front = NULL;
            _dynamicqueueitem * item = o._front;
            if (_size) {
                _front = new _dynamicqueueitem(item->_item);
                _end = _front;
                while (item->_next != NULL) {
                    item = item->_next;
                    _end->_next = new _dynamicqueueitem(item->_item);
                    _end = _end->_next;
                }
            }
        }

        void push(T t) {
            _dynamicqueueitem * item = new _dynamicqueueitem(t);
            if (_size) {
                _end->_next = item;
                _end = item;
            } else {
                _front = _end = item;
            }
            _size++;
        }

        void pop() {
            if (!_size) throw QueueError("Queue is empty.");
            _dynamicqueueitem * i = _front;
            _front = i->_next;
            delete i;
            _size--;
        }

        T front() {
            if (!_size) throw QueueError("Queue is empty");
            return _front->_item;
        }

        void clear() {
            while(!empty()) {
                pop();
            }
        }

        bool empty() {
            return _size == 0;
        }

        t_udword size() {
            return _size;
        }
    private:
        struct _dynamicqueueitem {
        public:
            _dynamicqueueitem(T item) {
                _item = item;
                _next = NULL;
            }
            T _item;
            _dynamicqueueitem * _next;
        };
        _dynamicqueueitem * _front, *_end;
        t_udword _size;
    };

    template <typename T, class Q>
    class tsqueue {
    public:
        tsqueue(t_udword size=_TTL_QUEUE_DEFAULT_SIZE) : _q(size) {}

        tsqueue(tsqueue<T, Q> & o) {
            o._mutex.lock();
            _q = o._q;
            o._mutex.unlock();
        }

        tsqueue(const tsqueue<T, Q> & o) {
            o._mutex.lock();
            _q = o._q;
            o._mutex.unlock();
        }

        tsqueue & operator=(tsqueue<T, Q> & o) {
            _mutex.lock();
            o._mutex.lock();
            _q = o._q;
            o._mutex.unlock();
            _mutex.unlock();
            return *this;
        }

        tsqueue & operator=(const tsqueue<T, Q> & o) {
            _mutex.lock();
            o._mutex.lock();
            _q = o._q;
            o._mutex.unlock();
            _mutex.unlock();
            return *this;
        }

        void push(T t) {
            _mutex.lock();
            _q.push(t);
            _mutex.unlock();
        }

        void pop() {
            _mutex.lock();
            _q.pop();
            _mutex.unlock();
        }

        T front() {
            _mutex.lock();
            T x = _q.front();
            _mutex.unlock();
            return x;
        }

        T front_and_pop() {
            _mutex.lock();
            T x = _q.front();
            _q.pop();
            _mutex.unlock();
            return x;
        }

        void clear() {
            _mutex.lock();
            _q.clear();
            _mutex.unlock();
        }

        bool empty() {
            _mutex.lock();
            bool b = _q.empty();
            _mutex.unlock();
            return b;
        }

        t_udword size() {
            _mutex.lock();
            t_udword s = _q.size();
            _mutex.unlock();
            return s;
        }
    private:
        Mutex _mutex;
        Q _q;
    };

    template<typename T>
    using tsfixedqueue = tsqueue<T, fixedqueue<T>>;

    template<typename T>
    using tsfixedgrowingqueue = tsqueue<T, fixedgrowingqueue<T>>;

    template<typename T>
    using tsdynamicqueue = tsqueue<T, dynamicqueue<T>>;
}

#endif //__TORUS_QUEUE_H
