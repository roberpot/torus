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

#ifndef __TORUS_CALLSTACK_H
#define __TORUS_CALLSTACK_H

#include <map>
#include <stack>
#include <queue>
#include <string>
#include <typeinfo>

//#include "<library/system_headers.h>
#include <threads/utils.h>
#include <threads/mutex.h>
#include <debug_support/debug.h>
#include <library/types.h>
#include <debug_support/info.h>

class StackEntry {
public:
    StackEntry(const t_byte * file, const t_byte * function, udword_t line);
    std::string _file, _function;
    udword_t _line;
};

extern class CallStack {
public:
    CallStack() {};
    void push(torus_thread_id tid, StackEntry e);
    void pop(torus_thread_id tid);
    void print(torus_thread_id tid);
private:
    CallStack(const CallStack & o);
    CallStack & operator=(const CallStack & o);
    std::map<torus_thread_id, std::stack<StackEntry> * > _stacks;
    std::map<torus_thread_id, std::queue<StackEntry> * > _tmp_queue;
    Mutex _m;
} _classtack;

class CallStackControl {
public:
    CallStackControl(const t_byte * file, const t_byte * function, udword_t line);
    ~CallStackControl();
};

#ifndef _DEBUG
    #define EXC_NOTIFY_DEBUGGER (void)0
#else	// we want the debugger to notice of this exception
    #ifdef _WIN32
        #ifdef _MSC_VER
            #define EXC_NOTIFY_DEBUGGER if (IsDebuggerPresent()) __debugbreak()
        #else
            #define EXC_NOTIFY_DEBUGGER if (IsDebuggerPresent()) abort()
        #endif
    #else
        #define EXC_NOTIFY_DEBUGGER if (IsDebuggerPresent()) std::raise(SIGINT)
    #endif
#endif

#if DEBUG_MODE > 0
#define ADDTOCALLSTACK() CallStackControl ___callstackcontrol(__FILE__, __func__, __LINE__)
#define EXC_TRY(a) \
    std::string ___current_block = ""; \
    std::string ___local_args = a; \
    udword_t ___block_count = 0; \
    bool ___catch_except = false; \
    UNREFERENCED_PARAMETER(___catch_except); \
    try \
    {
#define EXC_SET(a) \
___current_block = a; \
___block_count++;
#define EXC_CATCH } \
    catch (...) { EXC_CATCH_EXCEPTION(0); }
// Add other excepts -> catch ( const ExceptionClass& e )	{ EXC_CATCH_EXCEPTION(&e); }
#define EXC_CATCH_EXCEPTION(a) \
    ___catch_except = true; \
    _classtack.print(get_current_thread_id());\
    EXC_NOTIFY_DEBUGGER; \
    if ( ___current_block != "" && ___block_count > 0 ) {\
        TORUSSHELLECHO(((a != 0) ? a : "...") << " " << typeid(*this).name() << "::" << __func__ << "() " << ___local_args << " #" << ___block_count << " " << ___current_block); \
    } else {\
        TORUSSHELLECHO(((a != 0) ? a : "...") << " " << typeid(*this).name() << "::" << __func__ << "() " << ___local_args); \
    }
#define EXC_DEBUG_START \
    if (___catch_except) { try {
#define EXC_DEBUG_END \
	} \
    catch ( ... ) { DEBUG_ERROR("Exception adding debug_support message on the exception."); \
    } \
    }
#else //DEBUG_MODE > 0
#define ADDTOCALLSTACK()
#define EXC_TRY(a) {
#define EXC_SET(a)
#define EXC_CATCH }
#define EXC_DEBUG_START if (false) {
#define EXC_DEBUG_END }
#endif //DEBUG_MODE > 0

#endif //__TORUS_CALLSTACK_H
