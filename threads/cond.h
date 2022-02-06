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

#ifndef __TORUS_THREAD_COND_H
#define __TORUS_THREAD_COND_H

#include <threads/thread_defines.h>

class ConditionVariable {
public:
    /** @name Constructors, Destructor, Asign operator:
    */
    ///@{
    ConditionVariable();
    ~ConditionVariable();
private:
    ConditionVariable(const ConditionVariable& copy);
    ConditionVariable& operator=(const ConditionVariable& other);
    ///@}
public:
    /** @name Interaction:
    */
    ///@{
    void lock();
    void unlock();
    void wait();
    void signal();
    void broadcast();
private:
#ifdef __TORUS_THREAD_WINAPI
    CRITICAL_SECTION _mutex;   ///< Windows API specific mutex.
    CONDITION_VARIABLE _condvar;   ///< Windows API specific condition variable.
#endif
#ifdef __TORUS_THREAD_PTHREAD
    pthread_cond_t _condvar;   ///< Unix API specific condition variable.
    pthread_mutex_t _mutex;   ///< Unix API specific mutex.
    pthread_mutexattr_t _mutex_attr;   ///< Unix API mutex attr.
#endif
};

#endif //__TORUS_THREAD_COND_H
