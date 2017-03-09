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

#ifndef __TORUS_DB_OBJECT_H
#define __TORUS_DB_OBJECT_H

#include "../library/system_headers.h"
#include "pqxx/result.hxx"

class TDBObject {
public:
    TDBObject();
    virtual ~TDBObject();
    virtual bool db_load(pqxx::result::const_iterator r) = 0;
    virtual bool db_save() = 0;
    virtual void mark_db_update() = 0;
    virtual void mark_db_delete() = 0;
    bool db_update;    ///< True = flagged for update, don't try to add it again to it's _updatelist.
};
#endif // __TORUS_DB_OBJECT_H