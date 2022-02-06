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

#ifndef __TORUS_THREAD_THREAD_H
#define __TORUS_THREAD_THREAD_H

#include <threads/thread_defines.h>
#include <threads/thread.h>
#include <library/types.h>

class Thread {
public:
    /** @name Constructors, Destructor, Asign operator:
    */
    ///@{
    Thread();
    virtual ~Thread();
private:
    Thread(const Thread& copy);
    Thread& operator=(const Thread& other);
    ///@}
public:
    /** @name Interaction:
    */
    ///@{
    dword_t start();
    virtual void * run() = 0;
    dword_t stop();
    dword_t join();
    ///@}
private:
    static void * _run(void * c);
    bool _running;
    Mutex _mutex;
#ifdef __TORUS_THREAD_WINAPI
    HANDLE _thread;
#endif
#ifdef __TORUS_THREAD_PTHREAD
    pthread_t _thread;
    pthread_attr_t _attr;
#endif
};

#endif //__TORUS_THREAD_THREAD_H
