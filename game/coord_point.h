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

#ifndef __TORUS_GAME_COORD_POINT_H_
#define __TORUS_GAME_COORD_POINT_H_

#include <library/types.h>

class CoordPoint {
private:
    uword_t _x;    ///< X position.
    uword_t _y;    ///< Y position.
    t_byte _z;      ///< Z position.
    t_ubyte _m;     ///< Map index.
public:

    /**
     * @brief   Empty constructor for CoordPoint.
     * 
     */
    CoordPoint();

    /**
     * @brief   Constructor for CoordPoint for a given location
     * 
     * @param x     The X coordinate.
     * @param y     The Y coordinate.
     * @param z     The Z coordinate.
     * @param m     The Map index.
     */
    CoordPoint(const uword_t &x, const uword_t &y, const t_byte &z, const t_byte &m);

    /**
     * @brief Copy constructor for CoordPoint.
     * 
     * @param cp The CoordPoint to copy.
     */
    CoordPoint(const CoordPoint& cp);

    ~CoordPoint();

    /**
     * @brief Change the CoordPoint's location.
     *
     * @param x The new X coordinate.
     * @param y The new Y coordinate.
     * @return true if the new point is valid, false otherwise.
     */
    bool set_point(const uword_t& x, const uword_t& y);

    /**
     * @brief Change the CoordPoint's location.
     * 
     * @param x The new X coordinate.
     * @param y The new Y coordinate.
     * @param z The new Z altitude.
     * @return true if the new point is valid, false otherwise.
     */
    bool set_point(const uword_t& x, const uword_t& y, const t_byte& z);

    /**
     * @brief Change the CoordPoint's location.
     *
     * @param x The new X coordinate.
     * @param y The new Y coordinate.
     * @param z The new Z altitude.
     * @param m The new Map index.
     * @return true if the new point is valid, false otherwise.
     */
    bool set_point(const uword_t& x, const uword_t& y, const t_byte& z, const t_ubyte& m);

    /**
     * @brief Change the CoordPoints's location.
     * 
     * @param cp The CoordPoint to copy the location from.
     * @return true if the new point is valid, false otherwise.
     */
    bool set_z(const t_byte& cp);

    /**
     * @brief Change the CoordPoints's altitude.
     *
     * @param z The new altitude.
     * @return true if the new point is valid, false otherwise.
     */
    bool set_map(const t_byte& z);

    /**
     * @brief Change the CoordPoints's map.
     *
     * @param map The new map.
     * @return true if the new point is valid, false otherwise.
     */
    bool set_point(const CoordPoint& map);

    /**
     * @brief Get the X position.
     * 
     * @return The X.
     */
    uword_t get_x();
    /**
     * @brief Get the Y position.
     *
     * @return The Y.
     */
    uword_t get_y();
    /**
     * @brief Get the Z position.
     *
     * @return The Z.
     */
    t_byte get_z();
    /**
     * @brief Get the map.
     *
     * @return The map.
     */
    t_byte get_map();

    /**
     * @brief Check if this CoordPoint is valid.
     * 
     * This checks if the map is valid, the Z altiude is within it's ranges
     * and if the X and Y coordinates are within its map's bounds.
     * @return True if all checks are passed, false otherwise.
     */
    bool is_valid_point();

    /**
     * @brief Calculate the Euler distance between this point and another one.
     * 
     * @param target The target which distance has to be calculated.
     * @return The distance
     */
    uword_t get_distance(const CoordPoint &target);

    bool can_move_to_coord(const word_t& destX, const word_t& destY);
    bool can_move_to_z(const t_byte& destZ);
    bool can_move_to_map(const t_ubyte& destMap);
    bool can_move_to(const word_t& destX, const word_t& destY, const t_byte& destZ, const t_ubyte& destMap);
    bool can_move_to(const CoordPoint &cp);
};

#endif // __TORUS_GAME_COORD_POINT_H_
