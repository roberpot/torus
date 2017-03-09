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

#ifndef __TORUS_SYSTEM_HEADERS_
#define __TORUS_SYSTEM_HEADERS_

//  Some system headers must be included in certain in windows to avoid conflicts, it is important to do not alter them.
//  Also add any other dependancies here instead of adding them in their files and including these files here, so there won't be
//  problems with files requiring of both sockets and threads, for example, plus something new ( like pqxx requiring MIN/MAX not 
//  being defined before ) breaking the order of this chain.


//  OS includes START.
#ifdef _MSC_VER
    #pragma warning(disable: 4589)  // TODO: Fix warning instead of disabling
    #include "pqxx/connection"
    #include "pqxx/transaction"
    #pragma warning(default: 4589)
#endif
#include "../network/socket.h"
#include "../threads/thread.h"
#include "../threads/mutex.h"
// OS includes END.


#endif  //__TORUS_SYSTEM_HEADERS_