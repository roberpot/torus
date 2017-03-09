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

#include "torus_db_table.h"
#include "../debug/callstack.h"

TDBTable::TDBTable() {
}

TDBTable::~TDBTable() {
}

void TDBTable::save_all() {
    ADDTOCALLSTACK();
}

void TDBTable::update_obj(TDBObject * obj) {
    ADDTOCALLSTACK();
    if (!obj->db_update) {
        _updatelist.push_back(obj); // Add the obj to the _updatelist if it was not added before in this tick, the bool is cleared in TDBObject::db_save()
    }
}

void TDBTable::update() {
    ADDTOCALLSTACK();
    if (_updatelist.size()) {
        for (size_t i = 0; i < _updatelist.size(); i++) {
            TDBObject * obj = _updatelist[(t_udword)i];
            if (obj->db_save()) {
                obj->db_update = false;
            }
            else {
                // Something went wrong updating the database,
                // TODO: Save failed behaviour: should we load a restore point from the database to rollback this obj to a working state?
                // pending to add another vector to store failed objects if so.
            }
        }
        _updatelist.empty();
    }
}
