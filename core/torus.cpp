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

#include "torus.h"
#include "../debug/callstack.h"
#include "../threads/utils.h"

Torus::Torus() {
}

void Torus::stop() {
    ADDTOCALLSTACK();
    _run = false;
}

void Torus::set_thread_time(unsigned int id, unsigned int t) {
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
            torus_thread_sleep(100);
            _slaves_condvar.broadcast();
        }
        DEBUG_NOTICE("End of mainloop. Stopping slaves...");
        std::map<unsigned int, SlaveThread *>::iterator it = _slaves.begin(), end = _slaves.end();
        while(it != end) {
            it->second->halt();
            it++;
        }
        torus_thread_sleep(100);
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