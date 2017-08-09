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

#ifndef __TORUS_DB_TABLE_H
#define __TORUS_DB_TABLE_H

#include "torus_db_object.h"
#include "../library/vector.h"

class TDBTable {
    ttl::vector<TDBObject *> _updatelist;   ///< Vector storing the TDBOBjects that needs to be updated in the next save tick.
protected:
    std::string _table_name;
public:
    TDBTable();
    virtual ~TDBTable();
    virtual bool check() = 0;
    virtual void load_all() = 0;
    virtual void save_all();
    /**
     * @brief   Updates the object described by obj.
     * 
     * @param obj  The object to update.
     */
    void update_obj(TDBObject *obj);
    /**
     * @brief   Updates all the objects inside _updatelist (if any).
     */
    void update();
};

#endif // __TORUS_DB_TABLE_H
