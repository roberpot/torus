#include "map_point.h"
#include "../../debug/callstack.h"



MapPoint::MapPoint(t_uword tid, t_byte pz) {
    ADDTOCALLSTACK();
    tileid = tid;
    z = pz;
}

MapPoint::~MapPoint() {
    ADDTOCALLSTACK();
    _tiles.clear();
}

t_uword MapPoint::get_tileid() {
    ADDTOCALLSTACK();
    return tileid;
}

t_uword MapPoint::get_flags() {
    ADDTOCALLSTACK();
    return flags;
}

t_byte MapPoint::get_z() {
    ADDTOCALLSTACK();
    return z;
}

void MapPoint::add_static_tile(UOStaticTile &tile) {
    ADDTOCALLSTACK();
    _tiles.push_back(tile);
}

void MapPoint::add_item() {
    ADDTOCALLSTACK();
    // item not really stored, it's stored and used from DB, this is just an event to reapply walkflags
    reset_flags();
}

void MapPoint::del_item() {
    ADDTOCALLSTACK();
    reset_flags();
}

void MapPoint::reset_flags() {
    ADDTOCALLSTACK();
    t_byte minZ = z;    // map defines the lowest Z value to walk on.
    if (_tiles.size()) {
        for (size_t i = 0; i < _tiles.size(); i++) {

        }
    }
}