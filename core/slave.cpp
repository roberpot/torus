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

#include <ctime>
#include <core/slave.h>
#include <core/torus.h>
#include <debug_support/callstack.h>

udword_t SlaveThread::slave_id = 0;

SlaveThread::SlaveThread(ConditionVariable * cv) {
    _condvar = cv;
    _id = ++slave_id;
}

udword_t SlaveThread::id() {
    return _id;
}

void * SlaveThread::run() {
    ADDTOCALLSTACK();
    EXC_TRY("");
        _run = true;
        while (_run) {
            clock_t init_ticks = clock();
            torus.set_thread_time(_id, (udword_t)((float)(clock() - init_ticks) / CLOCKS_PER_SEC * 1000));
            _condvar->lock();
            _condvar->wait();
            _condvar->unlock();
        }
        DEBUG_NOTICE("Stopping slave: " << _id);
    EXC_CATCH;
    EXC_DEBUG_START;
    EXC_DEBUG_END;
    return NULL;
}

void SlaveThread::halt() {
    ADDTOCALLSTACK();
    _run = false;
}


void SlaveThread::delegate(SlaveThread * o) {
    UNREFERENCED_PARAMETER(o);
    // Try to split my work and give a portion to o.
}

void SlaveThread::delegate_all(SlaveThread * o) {
    UNREFERENCED_PARAMETER(o);
    // Give all my work to o.
}