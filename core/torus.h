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
#include <queue>
#include "../threads/utils.h"
#include "../threads/mutex.h"
#include "../threads/cond.h"
#include "slave.h"
#include "../library/types.h"

extern class Torus {
public:
    Torus();
    void stop();
    void set_thread_time(t_udword id, t_udword t);
    void mainloop();
    t_uqword get_serv_time();
private:
    void balance_control();
    bool _run;
    Mutex _time_map_mutex;
    ConditionVariable _slaves_condvar;
    std::map<t_udword, t_udword> _time_map;
    std::map<t_udword, SlaveThread *> _slaves;
    std::queue<SlaveThread *> _slaves_joinable;

    t_uqword _serv_time; ///< Server time in ticks.
} torus;

#endif //__TORUS_TORUS_H
