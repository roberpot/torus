#include "map_list.h"
#include <cstddef>
#include <cstring>
#include "../../debug/debug.h"
#include "../../debug/callstack.h"


MapList::~MapList() {
    ADDTOCALLSTACK();
    delete _map;
}

void MapList::init() {
    ADDTOCALLSTACK();
    //TODO: Read the amount of maps and their sizes to generate a proper list.
    _mapcount = 1;  // this should be set after the .ini is readed and all the required maps setup.
    _map = new Map[_mapcount];
    memset(_map, 0, sizeof(Map) * _mapcount);

    for (t_ubyte i = 0; i < _mapcount; i++) {
        _map[i].create(7168, 4096, i);
    }
}

Map &MapList::get_map(t_ubyte id) {
    ADDTOCALLSTACK();
    if (id >= _mapcount) {   // id out of limits
        DEBUG_ERROR("Trying to access to unexisting map " << id << " retrieving _map[0] instead.");
        return _map[0];
    }
    if (_map[id].get_id() == id)    // did I find a map with the ID I request?
        return _map[id];
    for (t_ubyte i = 0; i < _mapcount; i++) {   // seems like no, lets do a full search for it.
        if (_map[i].get_id() == id)
            return _map[i];
    }
    return _map[0]; //should not reach this, keeping to prevent warnings.
}

t_ubyte MapList::get_map_count() {
    ADDTOCALLSTACK();
    return _mapcount;
}
