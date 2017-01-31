#ifndef _TORUS_GAME_MAP_LIST_H
#define _TORUS_GAME_MAP_LIST_H

#include "../../core/types.h"
#include "map.h"

extern class MapList {
private:
    Map *_map;      ///<  array of Maps
    t_ubyte _mapcount = 0;  ///< count of maps
public:
    MapList() {}
    ~MapList();
    /**
    * @brief init this class and reads all the maps.
    */
    void init();
    /**
    * @brief gets the Map with the given id.
    *
    * Searchs for a valid _map[id] with _mapid = id, if that _map[]._mapid and id doesn't match it searchs with a for loop for a match.
    * @param id The ingame ID of this map.
    * @return The Map with this id.
    */
    Map &get_map(t_ubyte id);

    /**
    * @brief Get the total count of maps.
    *
    * @return N of maps.
    */
    t_ubyte get_map_count();
} maplist;

#endif //_TORUS_GAME_MAP_LIST_H