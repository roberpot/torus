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

#ifndef __TORUS_SLAVE_H
#define __TORUS_SLAVE_H

#include <library/types.h>
#include <threads/thread.h>
#include <threads/cond.h>

class SlaveThread : public Thread {
public:
    SlaveThread(ConditionVariable * cv);
    udword_t id();
    void * run();
    void halt();
    void delegate(SlaveThread * o);
    void delegate_all(SlaveThread * o);
private:
    static udword_t slave_id;
    udword_t _id;
    bool _run;
    ConditionVariable * _condvar;
};

#endif //__TORUS_SLAVE_H
