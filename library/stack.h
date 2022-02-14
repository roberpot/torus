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

#include <iostream>
#include <memory>

#include <library/types.h>
#include <threads/mutex.h>
#include <library/errors.h>
#include <library/memory.h>
#include <debug_support/info.h>

#ifdef _MSC_VER
    #pragma warning(disable: 4521 4522)  // TODO: Fix warnings instead of disabling.
#endif

#define _TTL_STACK_DEFAULT_SIZE 1

namespace ttl {
    template<typename T, class Allocator = std::allocator<T>>
    class staticstack {
    public:
        staticstack(udword_t size=_TTL_STACK_DEFAULT_SIZE);
        staticstack(const staticstack& o);
        staticstack(staticstack&& o);
        ~staticstack();
        staticstack & operator=(const staticstack& o);
        staticstack & operator=(staticstack&& o);

        // Element access.
        T& top();

        // Capacity.
        bool empty() const;
        udword_t size() const;

        // Modifiers.
        void push(const T& t);
        void push(T&& t);
        void pop();
        void clear();
        void swap(staticstack& o);

    private:
        T* _stack;
        udword_t _top;
        udword_t _capacity;
        Allocator _allocator;
    };

    template <typename T, class Allocator = std::allocator<T>>
    class staticgrowingstack {
    public:
        staticgrowingstack(udword_t size=_TTL_STACK_DEFAULT_SIZE);
        staticgrowingstack(const staticgrowingstack& o);
        staticgrowingstack(staticgrowingstack&& o);
        ~staticgrowingstack();
        staticgrowingstack& operator=(const staticgrowingstack& o);
        staticgrowingstack& operator=(staticgrowingstack&& o);

        // Element access.
        T& top();

        // Capacity.
        bool empty() const;
        udword_t size() const;

        // Modifiers.
        void push(const T& t);
        void push(T&& t);
        void pop();
        void clear();
        void swap(staticgrowingstack& o);

    private:
        T* _stack;
        udword_t _top;
        udword_t _capacity;
        Allocator _allocator;
    };

    template<typename T, class Allocator = std::allocator<T>>
    class dynamicstack {
    public:
        dynamicstack();
        // To allow thread secure wrapper constructor.
        dynamicstack(udword_t _);
        dynamicstack(const dynamicstack& o);
        dynamicstack(dynamicstack&& o);
        ~dynamicstack();
        dynamicstack& operator=(const dynamicstack& o);
        dynamicstack& operator=(dynamicstack&& o);

        // Element access.
        T& top();

        // Capacity.
        bool empty() const;
        udword_t size() const;

        // Modifiers.
        void push(const T& t);
        void push(T&& t);
        void pop();
        void clear();
        void swap(dynamicstack& o);

    private:
        struct _dynamicstacknode {
        public:
            _dynamicstacknode(const T& i, _dynamicstacknode* n = nullptr);
            _dynamicstacknode(T&& i, _dynamicstacknode* n = nullptr);
            ~_dynamicstacknode() {
                PRINT_INFO(item);
            }
            T item;
            _dynamicstacknode * next;
        };
        _dynamicstacknode* _top;
        udword_t _size;
    };

    template <typename T, class S>
    class __T_tsstack {
    public:
        __T_tsstack(udword_t size=_TTL_STACK_DEFAULT_SIZE);
        __T_tsstack(const __T_tsstack& o);
        __T_tsstack(__T_tsstack&& o);
        ~__T_tsstack();
        __T_tsstack & operator=(const __T_tsstack& o);
        __T_tsstack & operator=(__T_tsstack&& o);

        // Element access.
        T top();

        // Capacity.
        bool empty() const;
        udword_t size() const;

        // Modifiers.
        void push(const T& t);
        void push(T&& t);
        void pop();
        T top_and_pop();
        void clear();
        void swap(__T_tsstack& o);
    private:
        mutable Mutex _mutex;
        S _s;
    };

    template<typename T, class Allocator = std::allocator<T>>
    using tsfixedstack = __T_tsstack<T, staticstack<T,Allocator>>;

    template<typename T, class Allocator = std::allocator<T>>
    using tsfixedgrowingstack = __T_tsstack<T, staticgrowingstack<T,Allocator>>;

    template<typename T, class Allocator = std::allocator<T>>
    using tsdynamicstack = __T_tsstack<T, dynamicstack<T,Allocator>>;

    /**
     * Implementations
     */


    template<typename T, class Allocator>
    staticstack<T,Allocator>::staticstack(udword_t size) {
        _stack = _allocator.allocate(size);
        _top = 0;
        _capacity = size;
    }

    template<typename T, class Allocator>
    staticstack<T,Allocator>::staticstack(const staticstack& o) {
        _stack = _allocator.allocate(o._capacity);
        _top = o._top;
        _capacity = o._capacity;
        for(udword_t i = 0; i < _top; ++i) {
            _allocator.construct(&(_stack[i]), o._stack[i]);
        }
    }

    template<typename T, class Allocator>
    staticstack<T,Allocator>::staticstack(staticstack&& o) {
        _stack = o._stack;
        _top = o._top;
        _capacity = o._capacity;
        o._stack = nullptr;
        o._top = 0;
        o._capacity = 0;
    }

    template<typename T, class Allocator>
    staticstack<T,Allocator>::~staticstack() {
        if (nullptr != _stack) {
            for (udword_t i = 0; i < _top; ++i) {
                _allocator.destroy(&(_stack[i]));
            }
            _allocator.deallocate(_stack, _capacity);
        }
    }

    template<typename T, class Allocator>
    staticstack<T,Allocator>& staticstack<T,Allocator>::operator=(const staticstack& o) {
        if (this != &o) {
            clear();
            _allocator.deallocate(_stack, _capacity);
            _stack = _allocator.allocate(o._capacity);
            _top = o._top;
            _capacity = o._capacity;
            for(udword_t i = 0; i < _top; ++i) {
                _allocator.construct(&(_stack[i]), o._stack[i]);
            }
        }
        return *this;
    }

    template<typename T, class Allocator>
    staticstack<T,Allocator>& staticstack<T,Allocator>::operator=(staticstack&& o) {
        if (this != &o) {
            swap(o);
        }
        return *this;
    }

    template<typename T, class Allocator>
    T & staticstack<T,Allocator>::top()  {
        if (!_top) {
            throw StackError("stack is empty.");
        }
        return _stack[_top - 1];
    }

    template<typename T, class Allocator>
    bool staticstack<T,Allocator>::empty() const {
        return _top == 0;
    }

    template<typename T, class Allocator>
    udword_t staticstack<T,Allocator>::size() const {
        return _top;
    }

    template<typename T, class Allocator>
    void staticstack<T,Allocator>::push(const T& t) {
        if (_top == _capacity) {
            throw StackError("stack is full.");
        }
        _stack[_top++] = t;
    }

    template<typename T, class Allocator>
    void staticstack<T,Allocator>::push(T&& t) {
        if (_top == _capacity) {
            throw StackError("stack is full.");
        }
        _stack[_top++] = t;
    }

    template<typename T, class Allocator>
    void staticstack<T,Allocator>::pop() {
        if (!_top) {
            throw StackError("stack is empty.");
        }
        _allocator.destroy(&(_stack[--_top]));
    }

    template<typename T, class Allocator>
    void staticstack<T,Allocator>::clear() {
        while(!empty()) {
            pop();
        }
    }

    template<typename T, class Allocator>
    void staticstack<T,Allocator>::swap(staticstack & o) {
        if (this != &o) {
            T * aux = _stack;
            udword_t aux_capacity = _capacity;
            udword_t aux_top = _top;
            _stack = o._stack;
            _capacity = o._capacity;
            _top = o._top;
            o._stack = aux;
            o._capacity = aux_capacity;
            o._top = aux_top;
        }
    }

    template<typename T, class Allocator>
    staticgrowingstack<T,Allocator>::staticgrowingstack(udword_t size) {
        _stack = _allocator.allocate(size);
        _top = 0;
        _capacity = size;
    }

    template<typename T, class Allocator>
    staticgrowingstack<T,Allocator>::staticgrowingstack(const staticgrowingstack& o) {
        _stack = _allocator.allocate(o._capacity);
        _top = o._top;
        _capacity = o._capacity;
        for(udword_t i = 0; i < _top; ++i) {
            _allocator.construct(&(_stack[i]), o._stack[i]);
        }
    }

    template<typename T, class Allocator>
    staticgrowingstack<T,Allocator>::staticgrowingstack(staticgrowingstack&& o) {
        _stack = o._stack;
        _top = o._top;
        _capacity = o._capacity;
        o._stack = nullptr;
        o._top = 0;
        o._capacity = 0;
    }

    template<typename T, class Allocator>
    staticgrowingstack<T,Allocator>::~staticgrowingstack() {
        if (nullptr != _stack) {
            clear();
            _allocator.deallocate(_stack, _capacity);
        }
    }

    template<typename T, class Allocator>
    staticgrowingstack<T,Allocator>& staticgrowingstack<T,Allocator>::operator=(const staticgrowingstack& o) {
        if (this != &o) {
            clear();
            _allocator.deallocate(_stack, _capacity);
            _stack = _allocator.allocate(o._capacity);
            _top = o._top;
            _capacity = o._capacity;
            for(udword_t i = 0; i < _top; ++i) {
                _allocator.construct(&(_stack[i]), o._stack[i]);
            }
        }
        return *this;
    }

    template<typename T, class Allocator>
    staticgrowingstack<T,Allocator>& staticgrowingstack<T,Allocator>::operator=(staticgrowingstack&& o) {
        if (this != &o) {
            swap(o);
        }
        return *this;
    }

    template<typename T, class Allocator>
    T& staticgrowingstack<T,Allocator>::top() {
        if (!_top) {
            throw StackError("stack is empty.");
        }
        return _stack[_top - 1];
    }

    template<typename T, class Allocator>
    bool staticgrowingstack<T,Allocator>::empty() const {
        return _top == 0;
    }

    template<typename T, class Allocator>
    udword_t staticgrowingstack<T,Allocator>::size() const {
        return _top;
    }

    template<typename T, class Allocator>
    void staticgrowingstack<T,Allocator>::push(const T& t) {
        if (_top == _capacity) {
            udword_t  newcapacity = _capacity << 1;
            T * aux = _allocator.allocate(newcapacity);
            ttl::memory::memmove(aux, _stack, sizeof(T) * _capacity);
            _allocator.deallocate(_stack, _capacity);
            _stack = aux;
            _capacity = newcapacity;
        }
        _allocator.construct(&(_stack[_top++]), t);
    }

    template<typename T, class Allocator>
    void staticgrowingstack<T,Allocator>::push(T&& t) {
        if (_top == _capacity) {
            udword_t  newcapacity = _capacity << 1;
            T * aux = _allocator.allocate(newcapacity);
            ttl::memory::memmove(aux, _stack, sizeof(T) * _capacity);
            _allocator.deallocate(_stack, _capacity);
            _stack = aux;
            _capacity = newcapacity;
        }
        _allocator.construct(&(_stack[_top++]), t);
    }

    template<typename T, class Allocator>
    void staticgrowingstack<T,Allocator>::pop() {
        if (!_top) {
            throw StackError("stack is empty.");
        }
        _allocator.destroy(&(_stack[--_top]));
    }

    template<typename T, class Allocator>
    void staticgrowingstack<T,Allocator>::clear() {
        while(!empty()) {
            pop();
        }
    }

    template<typename T, class Allocator>
    void staticgrowingstack<T,Allocator>::swap(staticgrowingstack& o) {
        if (this != &o) {
            T * aux = _stack;
            udword_t aux_capacity = _capacity;
            udword_t aux_top = _top;
            _stack = o._stack;
            _capacity = o._capacity;
            _top = o._top;
            o._stack = aux;
            o._capacity = aux_capacity;
            o._top = aux_top;
        }
    }

    template<typename T, class Allocator>
    dynamicstack<T,Allocator>::dynamicstack() : _top(nullptr), _size(0) {
    }

    template<typename T, class Allocator>
    dynamicstack<T,Allocator>::dynamicstack(udword_t _) : dynamicstack() {
        UNREFERENCED_PARAMETER(_);
    }

    template<typename T, class Allocator>
    dynamicstack<T,Allocator>::dynamicstack(const dynamicstack& o) : _top(nullptr), _size(o._size) {
        _dynamicstacknode* source = o._top;
        _dynamicstacknode* prev = nullptr;
        _dynamicstacknode* aux;
        while (nullptr != source) {
            aux = new _dynamicstacknode(source->item);
            if (nullptr == _top) {
                _top = aux;
            } else {
                prev->next = aux;
            }
            prev = aux;
            source = source->next;
        }
    }

    template<typename T, class Allocator>
    dynamicstack<T,Allocator>::dynamicstack(dynamicstack&& o) : _top(o._top), _size(o._size) {
        o._top = nullptr;
        o._size = 0;
    }

    template<typename T, class Allocator>
    dynamicstack<T,Allocator>::~dynamicstack() {
        clear();
    }

    template<typename T, class Allocator>
    dynamicstack<T,Allocator>& dynamicstack<T,Allocator>::operator=(const dynamicstack& o) {
        if (this != &o) {
            clear();
            _size = o._size;
            _dynamicstacknode* source = o._top;
            _dynamicstacknode* prev = nullptr;
            _dynamicstacknode* aux;
            while (nullptr != source) {
                aux = new _dynamicstacknode(source->item);
                if (nullptr == _top) {
                    _top = aux;
                } else {
                    prev->next = aux;
                }
                prev = aux;
                source = source->next;
            }
        }
        return *this;
    }

    template<typename T, class Allocator>
    dynamicstack<T,Allocator>& dynamicstack<T,Allocator>::operator=(dynamicstack&& o) {
        if (this != &o) {
            swap(o);
        }
        return *this;
    }

    template<typename T, class Allocator>
    T& dynamicstack<T,Allocator>::top() {
        if (nullptr == _top) {
            throw StackError("stack is empty.");
        }
        return _top->item;
    }

    template<typename T, class Allocator>
    bool dynamicstack<T,Allocator>::empty() const {
        return nullptr == _top;
    }

    template<typename T, class Allocator>
    udword_t dynamicstack<T,Allocator>::size() const {
        return _size;
    }

    template<typename T, class Allocator>
    void dynamicstack<T,Allocator>::push(const T& t) {
        _top = new _dynamicstacknode(t, _top);
        _size++;
    }

    template<typename T, class Allocator>
    void dynamicstack<T,Allocator>::push(T&& t) {
        _top = new _dynamicstacknode(t, _top);
        _size++;
    }

    template<typename T, class Allocator>
    void dynamicstack<T,Allocator>::pop() {
        if (nullptr == _top) {
            throw StackError("stack is empty.");
        }
        _dynamicstacknode* top = _top;
        _top = top->next;
        delete top;
        _size--;
    }

    template<typename T, class Allocator>
    void dynamicstack<T,Allocator>::clear() {
        while(!empty()) {
            pop();
        }
    }

    template<typename T, class Allocator>
    void dynamicstack<T,Allocator>::swap(dynamicstack& o) {
        if (this != &o) {
            _dynamicstacknode* aux = _top;
            udword_t aux_size = _size;
            _top = o._top;
            _size = o._size;
            o._top = aux;
            o._size = aux_size;
        }

    }

    template<typename T, class Allocator>
    dynamicstack<T,Allocator>::_dynamicstacknode::_dynamicstacknode(const T& i, _dynamicstacknode * n) : item(i), next(n) {
    }

    template<typename T, class Allocator>
    dynamicstack<T,Allocator>::_dynamicstacknode::_dynamicstacknode(T&& i, _dynamicstacknode * n) : item(i), next(n) {
    }

    template <typename T, class S>
    __T_tsstack<T,S>::__T_tsstack(udword_t size) : _s(size) {}

    template <typename T, class S>
    __T_tsstack<T,S>::__T_tsstack(const __T_tsstack& o) {
        if (this != &o) {
            o._mutex.lock();
            _s = o._s;
            o._mutex.unlock();
        }
    }

    template <typename T, class S>
    __T_tsstack<T,S>::__T_tsstack(__T_tsstack&& o) {
        if (this != &o) {
            o._mutex.lock();
            _s.swap(o._s);
            o._mutex.unlock();
        }
    }

    template <typename T, class S>
    __T_tsstack<T,S>::~__T_tsstack() {
        _mutex.lock();
    }

    template <typename T, class S>
    __T_tsstack<T,S>& __T_tsstack<T,S>::operator=(const __T_tsstack& o) {
        if (this != &o) {
            _mutex.lock();
            o._mutex.lock();
            _s = o._s;
            o._mutex.unlock();
            _mutex.unlock();
        }
        return this;
    }

    template <typename T, class S>
    __T_tsstack<T,S>& __T_tsstack<T,S>::operator=(__T_tsstack&& o) {
        if (this != &o) {
            _mutex.lock();
            o._mutex.lock();
            _s.swap(o._s);
            o._mutex.unlock();
            _mutex.unlock();
        }
        return this;
    }

    template <typename T, class S>
    T __T_tsstack<T,S>::top() {
        _mutex.lock();
        T x = _s.top();
        _mutex.unlock();
        return x;
    }

    template <typename T, class S>
    bool __T_tsstack<T,S>::empty() const {
        _mutex.lock();
        bool b = _s.empty();
        _mutex.unlock();
        return b;
    }

    template <typename T, class S>
    udword_t __T_tsstack<T,S>::size() const {
        _mutex.lock();
        udword_t s = _s.size();
        _mutex.unlock();
        return s;
    }

    template <typename T, class S>
    void __T_tsstack<T,S>::push(const T& t) {
        _mutex.lock();
        _s.push(t);
        _mutex.unlock();
    }

    template <typename T, class S>
    void __T_tsstack<T,S>::push(T&& t) {
        _mutex.lock();
        _s.push(t);
        _mutex.unlock();
    }

    template <typename T, class S>
    void __T_tsstack<T,S>::pop() {
        _mutex.lock();
        _s.pop();
        _mutex.unlock();
    }

    template <typename T, class S>
    T __T_tsstack<T,S>::top_and_pop() {
        _mutex.lock();
        T t(_s.top());
        _s.pop();
        _mutex.unlock();
        return t;
    }

    template <typename T, class S>
    void __T_tsstack<T,S>::clear() {
        _mutex.lock();
        _s.clear();
        _mutex.unlock();
    }

    template <typename T, class S>
    void __T_tsstack<T,S>::swap(__T_tsstack& o) {
        if (this != &o) {
            _mutex.lock();
            o._mutex.lock();
            _s.swap(o._s);
            o._mutex.unlock();
            _mutex.unlock();
        }
    }

}

#ifdef _MSC_VER
    #pragma warning(disable: 4521 4522)
#endif

#endif //__TORUS_STACK_H
