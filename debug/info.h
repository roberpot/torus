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

#ifndef __TORUS_INFO_H
#define __TORUS_INFO_H

#ifdef UNREFERENCED_PARAMETER
#undef UNREFERENCED_PARAMETER
#endif //UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(x) (void)(x)

#ifdef UNREFERENCED_FUNCTION
#undef UNREFERENCED_FUNCTION
#endif //UNREFERENCED_FUNCTION
#ifdef _MSVC
#else
#define UNREFERENCED_FUNCTION(x) \
void __##x##_unused_function() { \
    void (*f)(void*) = (void (*)(void *))&x; \
    UNREFERENCED_PARAMETER(f); \
} \
void __##x##_unused_function() __attribute__ ((unused))
#endif

#endif //__TORUS_INFO_H
