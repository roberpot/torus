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
#include <cstdint>

// -- typedefs --
// byte / ubyte

typedef char t_byte;
typedef unsigned char t_ubyte;
// int / uint

typedef std::int16_t word_t;
typedef std::uint16_t uword_t;
// dword / udword

typedef std::int32_t dword_t;
typedef std::uint32_t udword_t;
// qword / uqword 

typedef std::int64_t qword_t;
typedef std::uint64_t uqword_t;
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
}

#endif //__TORUS_TYPES_H
