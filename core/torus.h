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

#ifndef __TORUS_TORUS_H
#define __TORUS_TORUS_H

#include <map>
#include "../threads/utils.h"
#include "../threads/mutex.h"
#include "../threads/cond.h"
#include "slave.h"

extern class Torus {
public:
    Torus();
    void stop();
    void set_thread_time(unsigned int id, unsigned int t);
    void mainloop();
private:
    bool _run;
    Mutex _time_map_mutex;
    ConditionVariable _slaves_condvar;
    std::map<unsigned int, unsigned int> _time_map;
    std::map<unsigned int, SlaveThread *> _slaves;
} torus;

#endif //__TORUS_TORUS_H
