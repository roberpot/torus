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

#include <iostream>
#include <memory>

#include <debug_support/info.h>
#include <library/errors.h>
#include <library/memory.h>
#include <threads/mutex.h>

#ifdef _MSC_VER
    #pragma warning(disable: 4521 4522)
#endif

#define _TTL_QUEUE_DEFAULT_SIZE 1

namespace ttl {
    template <typename T, class Allocator = std::allocator<T>>
    class fixedqueue {
    public:
        fixedqueue(udword_t size=_TTL_QUEUE_DEFAULT_SIZE);
        fixedqueue(const fixedqueue& o);
        fixedqueue(fixedqueue&& o);
        ~fixedqueue();
        fixedqueue& operator=(const fixedqueue& o);
        fixedqueue& operator=(fixedqueue&& o);

        // Element access.
        T& front();

        // Capacity.
        bool empty() const;
        udword_t size() const;
        udword_t capacity() const;

        // Modifiers
        void push(const T& t);
        void push(T&& t);
        void pop();
        void clear();
        void swap(fixedqueue& o);
    private:
        T* _queue;
        udword_t _front;
        udword_t _end;
        udword_t _capacity;
        bool _empty;
        Allocator _allocator;
    };

    template <typename T, class Allocator = std::allocator<T>>
    class fixedgrowingqueue {
    public:
        fixedgrowingqueue(udword_t size=_TTL_QUEUE_DEFAULT_SIZE);
        fixedgrowingqueue(const fixedgrowingqueue& o);
        fixedgrowingqueue(fixedgrowingqueue&& o);
        ~fixedgrowingqueue();
        fixedgrowingqueue& operator=(const fixedgrowingqueue& o);
        fixedgrowingqueue& operator=(fixedgrowingqueue&& o);

        // Element access.
        T& front();

        // Capacity.
        bool empty() const;
        udword_t size() const;
        udword_t capacity() const;

        // Modifiers.
        void push(const T& t);
        void push(T&& t);
        void pop();
        void clear();
        void swap(fixedgrowingqueue& o);

    private:
        T* _queue;
        udword_t _front;
        udword_t _end;
        udword_t _capacity;
        bool _empty;
        Allocator _allocator;
    };

    template <typename T, class Allocator = std::allocator<T>>
    class dynamicqueue {
    public:
        dynamicqueue();
        dynamicqueue(udword_t _);
        dynamicqueue(const dynamicqueue& o);
        dynamicqueue(dynamicqueue&& o);
        ~dynamicqueue();
        dynamicqueue & operator=(const dynamicqueue& o);
        dynamicqueue & operator=(dynamicqueue&& o);

        // Element access.
        T& front();

        // Capacity.
        bool empty() const;
        udword_t size() const;
        udword_t capacity() const;

        // Modifiers.
        void push(const T& t);
        void push(T&& t);
        void pop();
        void clear();
        void swap(dynamicqueue& o);

    private:
        struct _dynamicqueueitem {
        public:
            _dynamicqueueitem(const T& i, _dynamicqueueitem* n=nullptr);
            _dynamicqueueitem(T&& i, _dynamicqueueitem* n=nullptr);
            T item;
            _dynamicqueueitem * next;
        };
        _dynamicqueueitem* _front;
        _dynamicqueueitem* _end;
        udword_t _size;
    };

    template <typename T, class Q>
    class __T_tsqueue {
    public:
        __T_tsqueue(udword_t size=_TTL_QUEUE_DEFAULT_SIZE);
        __T_tsqueue(__T_tsqueue& o);
        __T_tsqueue(__T_tsqueue&& o);
        ~__T_tsqueue();
        __T_tsqueue& operator=(__T_tsqueue& o);
        __T_tsqueue& operator=(__T_tsqueue&& o);

        // Element access.
        T front();

        // Capacity.
        bool empty();
        udword_t size();
        udword_t capacity();

        // Modifiers.
        void push(const T& t);
        void push(T&& t);
        void pop();
        T front_and_pop();
        void clear();
        void swap(__T_tsqueue<T,Q>& o);
    private:
        Mutex _mutex;
        Q _q;
    };

    template<typename T, class Allocator = std::allocator<T>>
    using tsfixedqueue = __T_tsqueue<T, fixedqueue<T,Allocator>>;

    template<typename T, class Allocator = std::allocator<T>>
    using tsfixedgrowingqueue = __T_tsqueue<T, fixedgrowingqueue<T,Allocator>>;

    template<typename T, class Allocator = std::allocator<T>>
    using tsdynamicqueue = __T_tsqueue<T, dynamicqueue<T,Allocator>>;


    template <typename T, class Allocator>
    fixedqueue<T,Allocator>::fixedqueue(udword_t size) :
        _queue(nullptr), _front(0), _end(0), _capacity(size), _empty(true) {
        _queue = _allocator.allocate(size);
    }

    template <typename T, class Allocator>
    fixedqueue<T,Allocator>::fixedqueue(const fixedqueue& o) {
        _capacity = o._capacity;
        _front = 0;
        _end = (o._capacity + o._end - o._front) % o._capacity;
        _empty = o._empty;
        _queue = _allocator.allocate(_capacity);
        if (!_empty) {
            _allocator.construct(&(_queue[0]), o._queue[o._front]);
            for(udword_t i = 0; i % _capacity != _end; ++i) {
                _allocator.construct(&(_queue[i]), o._queue[(i + o._front) % _capacity]);
            }
        }
    }

    template <typename T, class Allocator>
    fixedqueue<T,Allocator>::fixedqueue(fixedqueue&& o) {
        _capacity = o._capacity;
        _front = o._front;
        _end = o._end;
        _empty = o._empty;
        o._queue = nullptr;
        o._front = 0;
        o._end = 0;
        o._empty = true;
    }

    template <typename T, class Allocator>
    fixedqueue<T,Allocator>::~fixedqueue() {
        if (nullptr != _queue) {
            clear();
            _allocator.deallocate(_queue, _capacity);
        }
    }

    template <typename T, class Allocator>
    fixedqueue<T,Allocator>& fixedqueue<T,Allocator>::operator=(const fixedqueue& o) {
        if (this != &o) {
            clear();
            _allocator.deallocate(_queue, _capacity);
            _capacity = o._capacity;
            _front = 0;
            _end = (o._capacity + o._end - o._front) % o._capacity;
            _empty = o._empty;
            _queue = _allocator.allocate(_capacity);
            if (!_empty) {
                _allocator.construct(&(_queue[0]), o._queue[o._front]);
                for(udword_t i = 1; i % _capacity != _end; ++i) {
                    _allocator.construct(&(_queue[i]), o._queue[(i + o._front) % _capacity]);
                }
            }
        }
        return *this;
    }

    template <typename T, class Allocator>
    fixedqueue<T,Allocator>& fixedqueue<T,Allocator>::operator=(fixedqueue&& o) {
        if (this != &o) {
            swap(o);
        }
        return *this;
    }

    template <typename T, class Allocator>
    T& fixedqueue<T,Allocator>::front() {
        if (false == _empty) {
            throw QueueError("queue is empty.");
        }
        return _queue[_front];
    }

    template <typename T, class Allocator>
    bool fixedqueue<T,Allocator>::empty() const {
        return _empty;
    }

    template <typename T, class Allocator>
    udword_t fixedqueue<T,Allocator>::size() const {
        if (_empty) {
            return 0;
        }
        if (_front == _end) {
            return _capacity;
        }
        return (_capacity + _end - _front) % _capacity;
    }

    template <typename T, class Allocator>
    udword_t fixedqueue<T,Allocator>::capacity() const {
        return _capacity;
    }

    template <typename T, class Allocator>
    void fixedqueue<T,Allocator>::push(const T& t) {
        if (_end == _front && !_empty) {
            throw QueueError("queue is full.");
        }
        _allocator.construct(&(_queue[_end]), t);
        _end = ++_end % _capacity;
        _empty = false;
    }

    template <typename T, class Allocator>
    void fixedqueue<T,Allocator>::push(T&& t) {
        if (_end == _front && !_empty) {
            throw QueueError("queue is full.");
        }
        _allocator.construct(&(_queue[_end]), t);
        _end = ++_end % _capacity;
        _empty = false;
    }

    template <typename T, class Allocator>
    void fixedqueue<T,Allocator>::pop() {
        if (_empty) {
            throw QueueError("queue is empty.");
        }
        _allocator.destroy(&(_queue[_front]));
        _front = (_front + 1) % _capacity;
        if (_front == _end) {
            _empty = true;
        }
    }

    template <typename T, class Allocator>
    void fixedqueue<T,Allocator>::clear() {
        while(!empty()) {
            pop();
        }
    }

    template <typename T, class Allocator>
    void fixedqueue<T,Allocator>::swap(fixedqueue& o) {
        if (this != &o) {
            T* aux = _queue;
            udword_t aux_front = _front;
            udword_t aux_end = _end;
            bool aux_empty = _empty;
            _queue = o._queue;
            _front = o._front;
            _end = o._end;
            _empty = o._empty;
            o._queue = aux;
            o._front = aux_front;
            o._end = aux_end;
            o._empty = aux_empty;
        }
    }

    template <typename T, class Allocator>
    fixedgrowingqueue<T,Allocator>::fixedgrowingqueue(udword_t size) :
        _queue(nullptr), _front(0), _end(0), _capacity(size), _empty(true) {
        _queue = _allocator.allocate(size);
    }

    template <typename T, class Allocator>
    fixedgrowingqueue<T,Allocator>::fixedgrowingqueue(const fixedgrowingqueue& o) {
        _capacity = o._capacity;
        _front = 0;
        _end = (o._capacity + o._end - o._front) % o._capacity;
        _empty = o._empty;
        _queue = _allocator.allocate(_capacity);
        if (!_empty) {
            _allocator.construct(&(_queue[0]), o._queue[o._front]);
            for(udword_t i = 1; i % _capacity != _end; ++i) {
                _allocator.construct(&(_queue[i]), o._queue[(i + o._front) % _capacity]);
            }
        }
    }

    template <typename T, class Allocator>
    fixedgrowingqueue<T,Allocator>::fixedgrowingqueue(fixedgrowingqueue&& o) {
        _queue = o._queue;
        _capacity = o._capacity;
        _front = o._front;
        _end = o._end;
        _empty = o._empty;
        o._queue = nullptr;
        o._capacity = 0;
        o._front = 0;
        o._end = 0;
        o._empty = true;
    }

    template <typename T, class Allocator>
    fixedgrowingqueue<T,Allocator>::~fixedgrowingqueue() {
        if (nullptr != _queue) {
            clear();
            _allocator.deallocate(_queue, _capacity);
        }
    }

    template <typename T, class Allocator>
    fixedgrowingqueue<T,Allocator> & fixedgrowingqueue<T,Allocator>::operator=(const fixedgrowingqueue& o) {
        if (this != &o) {
            clear();
            _allocator.deallocate(_queue, _capacity);
            _capacity = o._capacity;
            _front = 0;
            _end = (o._capacity + o._end - o._front) % o._capacity;
            _empty = o._empty;
            _queue = _allocator.allocate(_capacity);
            if (!_empty) {
                _allocator.construct(&(_queue[0]), o._queue[o._front]);
                for(udword_t i = 1; i % _capacity != _end; ++i) {
                    _allocator.construct(&(_queue[i]), o._queue[(i + o._front) % _capacity]);
                }
            }
        }
        return *this;
    }

    template <typename T, class Allocator>
    fixedgrowingqueue<T,Allocator> & fixedgrowingqueue<T,Allocator>::operator=(fixedgrowingqueue&& o) {
        if (this != &o) {
            swap(o);
        }
        return *this;
    }

    template <typename T, class Allocator>
    T& fixedgrowingqueue<T,Allocator>::front() {
        if (_empty) {
            throw QueueError("queue is empty.");
        }
        return _queue[_front];
    }

    template <typename T, class Allocator>
    bool fixedgrowingqueue<T,Allocator>::empty() const {
        return _empty;
    }

    template <typename T, class Allocator>
    udword_t fixedgrowingqueue<T,Allocator>::size() const {
        if (_empty) {
            return 0;
        }
        if (_front == _end) {
            return _capacity;
        }
        return (_capacity + _end - _front) % _capacity;
    }

    template <typename T, class Allocator>
    udword_t fixedgrowingqueue<T,Allocator>::capacity() const {
        return _capacity;
    }

    template <typename T, class Allocator>
    void fixedgrowingqueue<T,Allocator>::push(const T& t) {
        if (_end == _front && !_empty) {
            udword_t newcapacity = _capacity << 1;
            T * aux = _allocator.allocate(newcapacity);
            internal::memcpy(aux, &(_queue[_front]), sizeof(T) * (_capacity - _front));
            if (_end > 0) {
                internal::memcpy(&(aux[_capacity - _front]), _queue, sizeof(T) * _front);
            }
            _end = _capacity - 1;
            _front = 0;
            _allocator.deallocate(_queue, _capacity);
            _queue = aux;
            _capacity = newcapacity;
        }
        _allocator.construct(&(_queue[_end++]), t);
        _end %= _capacity;
        _empty = false;
    }

    template <typename T, class Allocator>
    void fixedgrowingqueue<T,Allocator>::push(T&& t) {
        if (_end == _front && !_empty) {
            udword_t newcapacity = _capacity << 1;
            T * aux = _allocator.allocate(newcapacity);
            memcpy(aux, &(_queue[_front]), sizeof(T) * (_capacity - _front));
            if (_end > 0) {
                memcpy(&(aux[_capacity - _front]), _queue, sizeof(T) * _front);
            }
            _end = _capacity - 1;
            _front = 0;
            _allocator.deallocate(_queue, _capacity);
            _queue = aux;
            _capacity = newcapacity;
        }
        _allocator.construct(&(_queue[_end++]), t);
        _end %= _capacity;
        _empty = false;
    }

    template <typename T, class Allocator>
    void fixedgrowingqueue<T,Allocator>::pop() {
        if (_empty) {
            throw QueueError("queue is empty.");
        }
        _front++;
        _front %= _capacity;
        if (_front == _end) {
            _empty = true;
        }
    }

    template <typename T, class Allocator>
    void fixedgrowingqueue<T,Allocator>::clear() {
        while(!empty()) {
            pop();
        }
    }

    template <typename T, class Allocator>
    void fixedgrowingqueue<T,Allocator>::swap(fixedgrowingqueue& o) {
        if (this != &o) {
            T* aux = _queue;
            udword_t aux_capacity = _capacity;
            udword_t aux_front = _front;
            udword_t aux_end = _end;
            bool aux_empty = _empty;
            _queue = o._queue;
            _capacity = o._capacity;
            _front = o._front;
            _end = o._end;
            _empty = o._empty;
            o._queue = aux;
            o._capacity = aux_capacity;
            o._front = aux_front;
            o._end = aux_end;
            o._empty = aux_empty;
        }
    }

    template <typename T, class Allocator>
    dynamicqueue<T,Allocator>::dynamicqueue() :
        _front(nullptr), _end(nullptr), _size(0) {
    }

    template <typename T, class Allocator>
    dynamicqueue<T,Allocator>::dynamicqueue(udword_t _) : dynamicqueue() {
        UNREFERENCED_PARAMETER(_);
    }

    template <typename T, class Allocator>
    dynamicqueue<T,Allocator>::dynamicqueue(const dynamicqueue& o) {
        _size = o._size;
        _end = _front = nullptr;
        _dynamicqueueitem * item = o._front;
        if (_size) {
            _front = new _dynamicqueueitem(item->item);
            _end = _front;
            while (nullptr != item->next) {
                item = item->next;
                _end->next = new _dynamicqueueitem(item->item);
                _end = _end->next;
            }
        }
    }

    template <typename T, class Allocator>
    dynamicqueue<T,Allocator>::dynamicqueue(dynamicqueue&& o) :
        _front(o._front), _end(o._end), _size(o._size) {
        o._front = nullptr;
        o._end = nullptr;
        o._size = 0;
    }

    template <typename T, class Allocator>
    dynamicqueue<T,Allocator>::~dynamicqueue() {
        clear();
    }

    template <typename T, class Allocator>
    dynamicqueue<T,Allocator>& dynamicqueue<T,Allocator>::operator=(const dynamicqueue& o) {
        if (this != &o) {
            clear();
            _size = o._size;
            _end = _front = nullptr;
            _dynamicqueueitem * item = o._front;
            if (_size) {
                _front = new _dynamicqueueitem(item->item);
                _end = _front;
                while (nullptr != item->next) {
                    item = item->next;
                    _end->next = new _dynamicqueueitem(item->item);
                    _end = _end->next;
                }
            }
        }
    }

    template <typename T, class Allocator>
    dynamicqueue<T,Allocator>& dynamicqueue<T,Allocator>::operator=(dynamicqueue&& o) {
        if (this != &o) {
            swap(o);
        }
    }

    template <typename T, class Allocator>
    T& dynamicqueue<T,Allocator>::front() {
        if (0 == _size) {
            throw QueueError("Queue is empty");
        }
        return _front->item;
    }

    template <typename T, class Allocator>
    bool dynamicqueue<T,Allocator>::empty() const {
        return _size == 0;
    }

    template <typename T, class Allocator>
    udword_t dynamicqueue<T,Allocator>::size() const {
        return _size;
    }

    template <typename T, class Allocator>
    udword_t dynamicqueue<T,Allocator>::capacity() const {
        return 0;
    }

    template <typename T, class Allocator>
    void dynamicqueue<T,Allocator>::push(const T& t) {
        _dynamicqueueitem* item = new _dynamicqueueitem(t);
        if (_size) {
            _end->next = item;
            _end = item;
        } else {
            _front = _end = item;
        }
        _size++;
    }

    template <typename T, class Allocator>
    void dynamicqueue<T,Allocator>::push(T&& t) {
        _dynamicqueueitem* item = new _dynamicqueueitem(t);
        if (_size) {
            _end->next = item;
            _end = item;
        } else {
            _front = _end = item;
        }
        _size++;
    }

    template <typename T, class Allocator>
    void dynamicqueue<T,Allocator>::pop() {
        if (0 == _size) {
            throw QueueError("Queue is empty.");
        }
        _dynamicqueueitem* i = _front;
        _front = i->next;
        delete i;
        _size--;
    }

    template <typename T, class Allocator>
    void dynamicqueue<T,Allocator>::clear() {
        while(!empty()) {
            pop();
        }
    }

    template <typename T, class Allocator>
    void dynamicqueue<T,Allocator>::swap(dynamicqueue& o) {
        if (this != &o) {
            _dynamicqueueitem* aux_front = _front;
            _dynamicqueueitem* aux_end = _end;
            uword_t aux_size = _size;
            _front = o._front;
            _end = o._end;
            _size = o._size;
            o._front = aux_front;
            o._end = aux_end;
            o._size = aux_size;
        }
    }

    template<typename T, class Allocator>
    dynamicqueue<T,Allocator>::_dynamicqueueitem::_dynamicqueueitem(const T& i, _dynamicqueueitem* n) : item(i), next(n) {
    }

    template<typename T, class Allocator>
    dynamicqueue<T,Allocator>::_dynamicqueueitem::_dynamicqueueitem(T&& i, _dynamicqueueitem* n) : item(i), next(n) {
    }

    template <typename T, class Q>
    __T_tsqueue<T,Q>::__T_tsqueue(udword_t size) : _q(size) {
    }

    template <typename T, class Q>
    __T_tsqueue<T,Q>::__T_tsqueue(__T_tsqueue& o) {
        o._mutex.lock();
        _q = o._q;
        o._mutex.unlock();
    }

    template <typename T, class Q>
    __T_tsqueue<T,Q>::__T_tsqueue(__T_tsqueue&& o) {
        o._mutex.lock();
        _q.swap(o._q);
        o._mutex.unlock();
    }

    template <typename T, class Q>
    __T_tsqueue<T,Q>::~__T_tsqueue() {
        _mutex.lock();
    }

    template <typename T, class Q>
    __T_tsqueue<T,Q>& __T_tsqueue<T,Q>::operator=(__T_tsqueue& o) {
        _mutex.lock();
        o._mutex.lock();
        _q = o._q;
        o._mutex.unlock();
        _mutex.unlock();
        return *this;
    }

    template <typename T, class Q>
    __T_tsqueue<T,Q>& __T_tsqueue<T,Q>::operator=(__T_tsqueue&& o) {
        _mutex.lock();
        o._mutex.lock();
        _q.swap(o._q);
        o._mutex.unlock();
        _mutex.unlock();
        return *this;
    }

    template <typename T, class Q>
    T __T_tsqueue<T,Q>::front() {
        _mutex.lock();
        T x = _q.front();
        _mutex.unlock();
        return x;
    }

    template <typename T, class Q>
    bool __T_tsqueue<T,Q>::empty() {
        _mutex.lock();
        bool b = _q.empty();
        _mutex.unlock();
        return b;
    }

    template <typename T, class Q>
    udword_t __T_tsqueue<T,Q>::size() {
        _mutex.lock();
        udword_t s = _q.size();
        _mutex.unlock();
        return s;
    }

    template <typename T, class Q>
    udword_t __T_tsqueue<T,Q>::capacity() {
        _mutex.lock();
        udword_t capacity = _q.capacity();
        _mutex.unlock();
        return capacity;
    }

    template <typename T, class Q>
    void __T_tsqueue<T,Q>::push(const T& t) {
        _mutex.lock();
        _q.push(t);
        _mutex.unlock();
    }

    template <typename T, class Q>
    void __T_tsqueue<T,Q>::push(T&& t) {
        _mutex.lock();
        _q.push(t);
        _mutex.unlock();
    }

    template <typename T, class Q>
    void __T_tsqueue<T,Q>::pop() {
        _mutex.lock();
        _q.pop();
        _mutex.unlock();
    }

    template <typename T, class Q>
    T __T_tsqueue<T,Q>::front_and_pop() {
        _mutex.lock();
        T x = _q.front();
        _q.pop();
        _mutex.unlock();
        return x;
    }

    template <typename T, class Q>
    void __T_tsqueue<T,Q>::clear() {
        _mutex.lock();
        _q.clear();
        _mutex.unlock();
    }

    template <typename T, class Q>
    void __T_tsqueue<T,Q>::swap(__T_tsqueue<T,Q>& o) {
        if (this != &o) {
            _mutex.lock();
            o._mutex.lock();
            _q.swap(o._q);
            o._mutex.unlock();
            _mutex.unlock();
        }
    }
}

#ifdef _MSC_VER
    #pragma warning(disable: 4521 4522)
#endif

#endif //__TORUS_QUEUE_H
