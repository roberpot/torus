#include "uo_file_reader.h"
#include <sstream>
#include "../../debug/debug.h"
#include "../../debug/callstack.h"
#include "../../core/torus.h"
#include "map_list.h"
#include "map.h"
#include "map_point.h"

UOFileReader uofilereader;

UOFileReader::UOFileReader() {
    ADDTOCALLSTACK();
    _size = 0;
}

UOFileReader::~UOFileReader() {
    ADDTOCALLSTACK();
}

std::string UOFileReader::get_file_name(UOFILE_IDS file, t_ubyte id, bool searchuop) {
    ADDTOCALLSTACK();
    switch (file) {
        case UOF_MULTI_MUL:
            return "multi.mul";
        case UOF_MULTI_IDX:
            return "multi.idx";
        case UOF_TILEDATA:
            return "tiledata.mul";
        case UOF_MAP:
        {
            std::stringstream ret;
            ret << "map" << (int)id;
            if (searchuop) {
                ret << "LegacyMUL.uop";
                return ret.str();
            }
            else {
                ret << ".mul";
                return ret.str();
            }
        }
        case UOF_STAIDX:
        {
            const t_byte *ret = "staidx%d.mul";
            printf_s(ret, id);
            return ret;
        }
        case UOF_STATIC:
        {
            const t_byte *ret = "statics%d.mul";
            printf_s(ret, id);
            return ret;
        }
    }
    return "null";
}

void UOFileReader::read_map(t_ubyte id, UOMapPoint **&data) {
    ADDTOCALLSTACK();
    if (maplist.get_map_count() <= id) {
        DEBUG_ERROR("Trying to read invalid map '" << id << "'");
        return;
    }
    t_ubyte mapid = maplist.get_map(id).get_file_id();
    std::string file_name = get_file_name(UOF_MAP, id);
    
    if (!_open(file_name.c_str())) {
        file_name = get_file_name(UOF_MAP, id, false);
        if (!_open(file_name.c_str())) {
            DEBUG_ERROR("Couldn't open map files (either .mul or .uop).");
            return;
        }
    }
    DEBUG_INFO("Begin reading " << file_name << " with a total of " << _size << " bytes.");
    t_uword blocks = (maplist.get_map(id).get_max_x() + maplist.get_map(id).get_max_y()) / UO_MAP_BLOCK_SIZE;

    t_uword pb = 0, // raw coord, before calculating the amount of X per each Y.
            px = 0, // coord x of this block (ingame coords).
            py = 0; // coord y.
    UOMapBlock block;
    DEBUG_INFO("Reading " << blocks -1 << " map blocks from map " << (int)id << "...");
    for (t_uword b = 0; b < blocks; b++) {  // blocks
        memset((char*)&block, 0, sizeof(UOMapBlock));   // clearing data.
        block.header = b * sizeof(UOMapBlock);          // setting the header to the correct position (block * blocks_size).
        if (!_seek(block.header, std::ios::beg)){
            DEBUG_ERROR("Error reading UOMapBlock '" << b << "' from file " << file_name);
            close();
            break;
        }
        if (!_read((char*)&block, sizeof(UOMapBlock))) {
            DEBUG_ERROR("Error reading UOMapBlock's cells '" << b << "' from file " << file_name);
            close();
            break;
        }
        pb = b * UO_MAP_BLOCK_SIZE; // point of this block (before converting it to X,Y coords).
        px = pb % maplist.get_map(id).get_max_x();  // pb % max.x points gives the x position
        py = pb / maplist.get_map(id).get_max_x();  // pb / max.x points fives the y position.
        t_uword cells = UO_MAP_BLOCK_SIZE * UO_MAP_BLOCK_SIZE;
        for (t_ubyte c = 0; c < cells; c++) {   // cells inside blocks.
            data[(t_uword)(px + (c / UO_MAP_BLOCK_SIZE))][(t_uword)(py +(c % UO_MAP_BLOCK_SIZE))] = block.points[c];
        }
    }
    DEBUG_INFO("Done reading Map " << (int)id);
    close();
}

bool UOFileReader::_open(const t_byte *file, t_udword flags) {
    if (is_open()) {
        DEBUG_ERROR("Trying to open a file when another one is already opened.");
        return false;
    }
    _size = get_size();
    open(file, flags);
    return is_open();
}

bool UOFileReader::_seek(t_uqword index, t_ubyte at) {
    ADDTOCALLSTACK();
    seekg(index, at);
    if (!tellg() == index) {
        return false;
    }
    return true;
}

bool UOFileReader::_read(t_byte * data, t_uqword cnt) {
    ADDTOCALLSTACK();
    read((char*)&data, cnt);
    /*if ( !data || !strlen(data) == cnt) {
        DEBUG_ERROR("Data's lenght not matching bytes count, invalid data. ");
        return false;
    }*/
    return true;
}

std::streampos UOFileReader::get_size() {
    ADDTOCALLSTACK();
    std::streampos curpos = tellg();
    seekg(0, std::ios::end);
    std::streampos size = tellg();
    seekg(0, std::ios::beg);
    return size;
}
