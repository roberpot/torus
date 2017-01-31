#ifndef _TORUS_GAME_MAP_H
#define _TORUS_GAME_MAP_H

#include "../../core/types.h"
#include "map_point.h"

#define UO_MAP_BLOCK_SIZE 8

// All these structures must be byte packed.
#if defined(_WIN32) && defined(_MSC_VER)
    // Microsoft dependant pragma
    #pragma pack(1)
    #define PACK_NEEDED
#else
    // GCC based compiler you can add:
    #define PACK_NEEDED __attribute__ ((packed))
#endif

/*
* 3 bytes
*/
struct UOMapPoint {
    t_uword terrain;
    t_byte z;
};

// Turn off structure packing.
#if defined(_WIN32) && defined(_MSC_VER)
    #pragma pack()
#else
    #undef PACK_NEEDED
#endif

/**
* 196 bytes per block = 8x8 tiles * UOMapPoint(3 bytes)
*/
struct UOMapBlock {
    t_udword header;  ///< unknown
    UOMapPoint points[UO_MAP_BLOCK_SIZE * UO_MAP_BLOCK_SIZE];   ///< array of tiles
};

class Map {
private:
    t_uword _x = 0;     // max X
    t_uword _y = 0;     // max Y
    t_ubyte _mapid = 0;     // ingame .map
    t_ubyte _fileid = 255;    // map file in which this map is based on (255 = bad id).
    //MapPoint **_mappoint;
    UOMapPoint **_mappoint;
public:
    Map();
    ~Map();
    void create(t_word x, t_word y, t_ubyte id);
    t_word get_max_x();
    t_word get_max_y();
    t_ubyte get_id();
    t_ubyte get_file_id();
    bool is_valid_p(t_word x, t_word y);
    bool read_from_file();
    UOMapPoint get_map_point(t_word x, t_word y);
    //MapPoint get_map_point(t_word x, t_word y);
};

#endif //_TORUS_GAME_MAP_H