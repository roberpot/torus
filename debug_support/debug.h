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

#ifndef __TORUS_DEBUG_H
#define __TORUS_DEBUG_H

#ifndef DEBUG_MODE
#define DEBUG_MODE 0
#endif //DEBUG_MODE

#if DEBUG_MODE > 0
#include <string>
#include <sstream>
#include <shell.h>
#include <library/string.h>
#define _DBG_MSG(x, y) TORUSSHELLECHO(x << "::" << remove_prefix(SOURCE_PATH, __FILE__) << "(" << __LINE__ << ")::" << __func__ << ": " << y);
#else //DEBUG_MODE > 0
#define _DBG_MSG(x, y)
#define debug_init()
#define debug_halt()
#endif


#define DEBUG_ERROR(x)
#define DEBUG_WARNING(x)
#define DEBUG_NOTICE(x)
#define DEBUG_INFO(x)
#define DEBUG_MSG(x)

#if DEBUG_MODE > 0
#undef DEBUG_ERROR
#define DEBUG_ERROR(x) _DBG_MSG("[ERROR]", x)
#endif
#if DEBUG_MODE > 1
#undef DEBUG_WARNING
#define DEBUG_WARNING(x) _DBG_MSG("[WARNING]", x)
#endif
#if DEBUG_MODE > 2
#undef DEBUG_NOTICE
#define DEBUG_NOTICE(x) _DBG_MSG("[NOTICE]", x)
#endif
#if DEBUG_MODE > 3
#undef DEBUG_INFO
#define DEBUG_INFO(x) _DBG_MSG("[INFO]", x)
#endif
#if DEBUG_MODE > 4
#undef DEBUG_MSG
#define DEBUG_MSG(x) _DBG_MSG("[MSG]", x)
#endif

#endif //__TORUS_DEBUG_H

