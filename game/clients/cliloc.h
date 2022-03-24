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

#ifndef __TORUS_GAME_CLIENTS_CLILOC_H_
#define __TORUS_GAME_CLIENTS_CLILOC_H_

#include <library/types.h>
#include <game/clients/cliloc_variant.h>

#include <vector>
#include <string>

class Cliloc {
    udword_t _id;
    std::vector<ClilocVariant> _args;
    bool _has_personal_data;
public:
    Cliloc() {};
    /**
     * @brief Constructs a cliloc
     * 
     * @param id The cliloc ID
     * @param personal_data Wether this cliloc has personal data or not*
     * 
     * *Personal data means per-player specific informacion like, for example, the ReqStr cliloc
     * which must be red for players not having the required value and white for the rest.
     */
    Cliloc(const udword_t& id, bool personal_data = false);
    Cliloc(const Cliloc& other);
    Cliloc operator= (const Cliloc& other);
    udword_t get_id() const;
    void add_arg(const std::string& str);
    void add_arg(const dword_t& val);
    uword_t get_arg_count() const;
    std::string get_args() const;

    /**
     * @brief Get if this Cliloc has personal data*.
     * 
     * @return True if the Cliloc has personal data, false otherwise.
     * 
     * *Personal data means per-player specific informacion like, for example, the ReqStr cliloc
     * which must be red for players not having the required value and white for the rest.
     */
    bool has_personal_data() const;
};

#endif  // __TORUS_GAME_CLIENTS_CLILOC_H_
