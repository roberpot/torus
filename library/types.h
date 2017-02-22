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

#ifndef __TORUS_TYPES_H
#define __TORUS_TYPES_H
#include <climits>

// -- typedefs --
// byte / ubyte

typedef char t_byte;
typedef unsigned char t_ubyte;
// int / uint

typedef short int t_word;
typedef unsigned short int t_uword;
// dword / udword

typedef int t_dword;
typedef unsigned int t_udword;
// qword / uqword 

typedef long long t_qword;
typedef unsigned long long t_uqword;
// -- limits --
// byte / ubyte

#define TBYTE_MIN SCHAR_MIN
#define TBYTE_MAX SCHAR_MAX
#define TUBYTE_MAX UCHAR_MAX
// int / uint

#define TWORD_MIN SHRT_MIN
#define TWORD_MAX SHRT_MAX
#define TUWORD_MAX USHRT_MAX
// dword / udword

#define TDWORD_MIN INT_MIN
#define TDWORD_MAX INT_MAX
#define TUDWORD_MAX UINT_MAX
// qword / uqword 

#define TQWORD_MIN LLONG_MIN
#define TQWORD_MAX LLONG_MAX
#define TUQWORD_MAX ULLONG_MAX

namespace ttl {
    typedef long long ptrdiff_t;
}

#endif //__TORUS_TYPES_H
