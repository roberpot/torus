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

#ifndef __TORUS_THREAD_UTILS_H
#define __TORUS_THREAD_UTILS_H

#include "thread_defines.h"
#ifdef __TORUS_THREAD_WINAPI
typedef DWORD torus_thread_id;
#endif //__TORUS_THREAD_WINAPI
#ifdef __TORUS_THREAD_PTHREAD
typedef pthread_t torus_thread_id;

#endif //__TORUS_THREAD_PTHREAD

torus_thread_id get_current_thread_id();
void torus_thread_sleep(unsigned int m);

#endif //__TORUS_THREAD_UTILS_H
