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

#ifndef __TORUS_GAME_CLIENTS_TOOLTIP_H_
#define __TORUS_GAME_CLIENTS_TOOLTIP_H_

#include <library/types.h>
#include <game/clients/cliloc.h>

#include <map>
#include <string>
#include <vector>

class Tooltip {
    std::map<udword_t, Cliloc> _clilocs;
    bool _has_personal_data;
    udword_t _version;
public:
    Tooltip();
    Tooltip(const Tooltip& other);

    /**
     * @brief Adds a cliloc to this tooltip*
     * 
     * @param cliloc The cliloc to add.
     * 
     * *Note that the same cliloc may be added several times but the client won't show it.
     */
    void add_cliloc(const Cliloc& cliloc);

    /**
     * @brief Get the count of Clilocs of this tooltip.
     * 
     * @return The count.
     */
    size_t get_cliloc_count();

    /**
     * @brief Get the list of clilocs of this Tooltip.
     * 
     * @return The list.
     */
    std::vector<Cliloc> get_clilocs() const;

    /**
     * @brief Get if this Cliloc has personal data*.
     *
     * @return True if the Cliloc has personal data, false otherwise.
     *
     * *Personal data means per-player specific informacion like, for example, the ReqStr cliloc
     * which must be red for players not having the required value and white for the rest.
     */
    bool has_personal_data() const;

    udword_t get_version() const;
};

#endif  // __TORUS_GAME_CLIENTS_TOOLTIP_H_
