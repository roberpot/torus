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

#include <iomanip>

#include "callstack.h"
#include "../shell.h"
#include "../library/types.h"

CallStack _classtack;
std::string prefix = SOURCE_PATH;

StackEntry::StackEntry(const t_byte * file, const t_byte * function, t_udword line) {
    _file = file;
    _file.erase(0, prefix.length());
    _function = function;
    _line = line;
}

void CallStack::push(torus_thread_id tid, StackEntry e) {
    _m.lock();
    if (_stacks.count(tid) == 0) {
        _stacks[tid] = new std::stack<StackEntry>;
        _tmp_queue[tid] = new std::queue<StackEntry>;
    }
    _stacks[tid]->push(e);
    while (!_tmp_queue[tid]->empty()) _tmp_queue[tid]->pop();
    _m.unlock();
}

void CallStack::pop(torus_thread_id tid) {
    _m.lock();
    std::stack<StackEntry> * s = _stacks[tid];
    _tmp_queue[tid]->push(s->top());
    s->pop();
    if (s->empty()) {
        _stacks.erase(tid);
        _tmp_queue.erase(tid);
        delete s;
    }
    _m.unlock();
}

void CallStack::print(torus_thread_id tid) {
    _m.lock();
    std::stack<StackEntry> * s = _stacks[tid], s2;
    std::queue<StackEntry> * q = _tmp_queue[tid];
    TORUSSHELLECHO("_Thread__________ | #  | _file_________________________ | _line_ | _function___________ | ticks ");
    size_t count = s->size();
    while(!q->empty()) {
        s2.push(q->front());
        q->pop();
    }
    while(s->size()) {
        s2.push(s->top());
        s->pop();
    }
    for (t_udword i = 0; s2.size(); i++) {
        StackEntry e = s2.top();
        TORUSSHELLECHO(">> 0x" << std::hex << tid << " | " << std::setw(2) << std::right << i  << " | " << std::setw(30) << std::left << e._file << " | "<< std::setw(6) << std::dec << std::right << e._line << " | " << std::setw(20) << std::left << e._function << " | ??  " << (( i == (count - 1) ) ? "<-- exception catch point" : ""));
        s2.pop();
        if (i <= count) s->push(e);
    }
    _m.unlock();
}

CallStackControl::CallStackControl(const t_byte * file, const t_byte * function, t_udword line) {
    StackEntry e(file, function, line);
    _classtack.push(get_current_thread_id(), e);
}
CallStackControl::~CallStackControl() {
    _classtack.pop(get_current_thread_id());
}
