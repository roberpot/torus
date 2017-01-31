#include "map.h"
#include "../../debug/callstack.h"
#include "uo_file_reader.h"

Map::Map(){
}

Map::~Map(){
    ADDTOCALLSTACK();
    for (t_word i = 0; i < _x; i++) {
        delete _mappoint[i];
    }
    delete _mappoint;
}

void Map::create(t_word x, t_word y, t_ubyte id){
    ADDTOCALLSTACK();
    _mapid = id;
    _x = x;
    _y = y;
    /*_mappoint = new MapPoint*[x];
    for (t_word i = 0; i < x; i++) {
        _mappoint[i] = new MapPoint[y];
        memset(_mappoint[i], 0, sizeof(MapPoint) * y);
    }*/

    _mappoint = new UOMapPoint*[x];
    for (t_word i = 0; i < x; i++) {
        _mappoint[i] = new UOMapPoint[y];
        memset(_mappoint[i], 0, sizeof(UOMapPoint) * y);
    }
    read_from_file();
}

t_word Map::get_max_x(){
    ADDTOCALLSTACK();
    return _x;
}

t_word Map::get_max_y(){
    ADDTOCALLSTACK();
    return _y;
}

t_ubyte Map::get_id(){
    ADDTOCALLSTACK();
    return _mapid;
}

t_ubyte Map::get_file_id() {
    ADDTOCALLSTACK();
    return _fileid;
}

bool Map::is_valid_p(t_word x, t_word y){
    ADDTOCALLSTACK();
    if (x <= 0 || y <= 0
        || x > _x || y > _y)
        return false;
    return true;
}

bool Map::read_from_file(){
    ADDTOCALLSTACK();
    //TODO FILEREADER: read from mapXX.mul/uop and fill _flags cache with it's data.

    //TODO FILEREADER: read from statics file to add statics items to the map's cache and refresh _flags.
    uofilereader.read_map(0, _mappoint);
    return true;
}

UOMapPoint Map::get_map_point(t_word x, t_word y) {
    ADDTOCALLSTACK();
    return _mappoint[x][y];
}
