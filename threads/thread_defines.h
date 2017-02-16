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

#ifndef __TORUS_THREAD_DEFINES_H
#define __TORUS_THREAD_DEFINES_H

#ifdef _WINDOWS
    #ifdef __MINGW32__
        #define __TORUS_THREAD_PTHREAD
    #else
        #define __TORUS_THREAD_WINAPI
    #endif
#else
#define __TORUS_THREAD_PTHREAD
#endif

#ifdef __TORUS_THREAD_WINAPI
#include <windows.h>
#endif
#ifdef __TORUS_THREAD_PTHREAD
#include <pthread.h>
#include <unistd.h>
#endif

#endif //__TORUS_THREAD_DEFINES_H
