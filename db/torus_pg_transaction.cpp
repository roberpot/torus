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

#include "torus_pg_transaction.h"

PGTransaction::PGTransaction(pqxx::connection * conn, std::string query) : pqxx::work(*conn) , pqxx::internal::namedclass("TTSTransaction") {
    _conn = conn;
}

PGTransaction::~PGTransaction() {
    _conn = 0;
}