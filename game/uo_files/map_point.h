#ifndef _TORUS_GAME_MAP_POINT_H
#define _TORUS_GAME_MAP_POINT_H

#include "../../core/types.h"
#include "static_tile.h"
#include <vector>

class MapPoint {
private:
    t_uword tileid; // map file value.
    t_byte z;       // map file value.
    t_uword flags = 0;  // custom value, fast walkchecks.
    std::vector<UOStaticTile> _tiles;   // statics file items.
public:
    MapPoint(t_uword tid, t_byte pz);
    ~MapPoint();
    t_uword get_tileid();
    t_uword get_flags();
    t_byte get_z();
    void add_static_tile(UOStaticTile &tile);
    void add_item();
    void del_item();
    void reset_flags();
};

#endif //_TORUS_GAME_MAP_POINT_H