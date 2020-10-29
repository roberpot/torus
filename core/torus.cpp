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

#include <library/system_headers.h>
#include <core/torus.h>
#include <debug_support/callstack.h>
#include <threads/utils.h>
#include <core/config.h>
#include <game/server.h>

Torus::Torus() {
}

void Torus::stop() {
    ADDTOCALLSTACK();
    _run = false;
}

void Torus::set_thread_time(udword_t id, udword_t t) {
    ADDTOCALLSTACK();
    _time_map_mutex.lock();
    _time_map[id] = t;
    _time_map_mutex.unlock();
}

void Torus::mainloop() {
    ADDTOCALLSTACK();
    EXC_TRY("");
        _run = true;
        SlaveThread * thread = new SlaveThread(&_slaves_condvar);
        _slaves[thread->id()] = thread;
        _time_map[thread->id()] = 0;
        thread->start();
        DEBUG_NOTICE("Start of mainloop.");
        while(_run) {
            torus_thread_sleep(toruscfg.tick_duration);
            balance_control();
            _slaves_condvar.broadcast();
            //server._serv_time++;
        }
        DEBUG_NOTICE("End of mainloop. Stopping slaves...");
        std::map<udword_t, SlaveThread *>::iterator it = _slaves.begin(), end = _slaves.end();
        while(it != end) {
            it->second->halt();
            it++;
        }
        torus_thread_sleep(toruscfg.tick_duration);
        _slaves_condvar.broadcast();
        it = _slaves.begin();
        while(it != end) {
            it->second->join();
            delete it->second;
            it++;
        }
        DEBUG_NOTICE("End of mainloop. Stopping slaves... OK");
    EXC_CATCH;
    EXC_DEBUG_START;
    EXC_DEBUG_END;
}

void Torus::balance_control() {
    ADDTOCALLSTACK();
    std::queue<SlaveThread *> slaves_idle, slaves_overloaded;
    SlaveThread * thread;
    // First, remove joinable slaves.
    while (_slaves_joinable.size()) {
        thread = _slaves_joinable.front();
        _slaves_joinable.pop();
        thread->join();
        _slaves.erase(thread->id());
        _time_map.erase(thread->id());
        delete thread;
    }
    // Now check times.
    std::map<unsigned int, unsigned int>::iterator i = _time_map.begin(), e = _time_map.end();
    while(i != e) {
        if (i->second < toruscfg.tick_duration_idle) {
            slaves_idle.push(_slaves[i->first]);
        } else if (i->second > toruscfg.tick_duration_overloaded) {
            slaves_overloaded.push(_slaves[i->first]);
        }
        i++;
    }
    // Now try to split overloaded slaves.
    while(slaves_overloaded.size()) {
        thread = slaves_overloaded.front();
        slaves_overloaded.pop();
        // If any idle thread, delegate work on it.
        if (slaves_idle.size()) {
            SlaveThread * o = slaves_idle.front();
            DEBUG_INFO("Thread " << thread->id() << " overloaded! Delegating on thread " << o->id());
            slaves_idle.pop();
            thread->delegate(o);
        } else {
            // If not idle threads, create a new thread.
            DEBUG_INFO("Thread " << thread->id() << " overloaded! Delegating on a new thread.");
            SlaveThread * o = new SlaveThread(&_slaves_condvar);
            _slaves[o->id()] = thread;
            _time_map[o->id()] = 0;
            o->start();
            thread->delegate(o);
        }
    }
    // Now try to fuse idle threads.
    while(slaves_idle.size() > 1) {
        SlaveThread * t1, * t2;
        t1 = slaves_idle.front();
        slaves_idle.pop();
        t2 = slaves_idle.front();
        slaves_idle.pop();
        DEBUG_INFO("Threads idle " << t1->id() << " and " << t2->id() << ". Delegating all to " << t2->id() << ".");
        t1->delegate_all(t2);
        t1->halt();
        _slaves_joinable.push(t1);
    }
}
