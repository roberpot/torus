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

#include <debug_support/callstack.h>
#include <game/uo_files/map_list.h>
#include <game/uo_files/map.h>
#include "coord_point.h"

CoordPoint::CoordPoint() :
    _x(633),
    _y(858),
    _z(0),
    _m(0)
{
}

CoordPoint::CoordPoint(const uword_t &x, const  uword_t &y, const  t_byte &z, const t_byte &m) :
    _x(x),
    _y(y),
    _z(z),
    _m(m)
{
}

CoordPoint::CoordPoint(const CoordPoint& cp) :
    _x(cp._x),
    _y(cp._y),
    _z(cp._z),
    _m(cp._m)
{
}

CoordPoint::~CoordPoint()
{
}

bool CoordPoint::set_point(const uword_t& x, const uword_t& y)
{
    _x = x;
    _y = y;
    return is_valid_point();
}

bool CoordPoint::set_point(const uword_t& x, const uword_t& y, const t_byte& z)
{
    _x = x;
    _y = y;
    _z = z;
    return is_valid_point();
}

bool CoordPoint::set_point(const uword_t& x, const uword_t& y, const t_byte& z, const t_ubyte& m)
{
    _x = x;
    _y = y;
    _z = z;
    _m = m;
    return is_valid_point();
}

bool CoordPoint::set_z(const t_byte& z)
{
    if (!can_move_to_z(z))
    {
        return false;
    }
    _z = z;
    return true;
}

bool CoordPoint::set_map(const t_byte& map)
{
    if (!can_move_to_map(map))
    {
        return false;
    }
    _m = map;
    return true;
}

bool CoordPoint::set_point(const CoordPoint& cp)
{
    if (!can_move_to(cp))
    {
        return false;
    }
    _x = cp._x;
    _y = cp._y;
    _z = cp._z;
    _m = cp._m;
    return is_valid_point();
}

uword_t CoordPoint::get_x()
{
    return _x;
}

uword_t CoordPoint::get_y()
{
    return _y;
}

t_byte CoordPoint::get_z()
{
    return _z;
}

t_byte CoordPoint::get_map()
{
    return _m;
}

bool CoordPoint::is_valid_point() const
{
    bool is_valid = false;
    //if (maplist.is_map_valid(_m))
    {
        Map *map = maplist.get_map(_m);
        if (can_move_to_z(_z))
        {
            if (map->is_valid_p(_x, _y))
            {
                is_valid = true;
            }
        }
    }
    return is_valid;
}

uword_t CoordPoint::get_distance(const CoordPoint& target) const
{
    return uword_t(std::abs(_x - target._x) + std::abs(_y - target._y));
}

bool CoordPoint::can_move_to_coord(const word_t& destX, const word_t& destY) const {
    ADDTOCALLSTACK();
    Map* map = maplist.get_map(_m);
    if (map == nullptr) {
        //err
        return false;
    }
    if (!map->is_valid_p(destX, destY)) {
        return false;
    }
    /*
    if (map.get_flags() & MAPFLAGS_BLOCK){
        return false;    // TODO: Read map to set flags and create internal flags to do checks against them
    }
    */
    return true;
}

bool CoordPoint::can_move_to_z(const t_byte& destZ) const {
    ADDTOCALLSTACK();
    if (destZ < TBYTE_MIN || destZ > TBYTE_MAX) {
        return false;
    }
    return true;
}

bool CoordPoint::can_move_to_map(const t_ubyte& destMap) const {
    ADDTOCALLSTACK();
    return maplist.is_map_valid(destMap);
}

bool CoordPoint::can_move_to(const word_t& destX, const word_t& destY, const t_byte& destZ, const t_ubyte& destMap) const {
    ADDTOCALLSTACK();
    return can_move_to_coord(destX, destY) && can_move_to_z(destZ) && can_move_to_map(destMap);
}

bool CoordPoint::can_move_to(const CoordPoint& cp) const
{
    ADDTOCALLSTACK();
    return can_move_to(cp._x, cp._y, cp._z, cp._m);
}
