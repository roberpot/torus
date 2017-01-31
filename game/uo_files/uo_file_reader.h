#ifndef _TORUS_GAME_UO_FILE_READER_H
#define _TORUS_GAME_UO_FILE_READER_H

#include <fstream>
#include "../../core/types.h"
#include "map.h"

class MapPoint;

enum UOFILE_TYPES {
    UOFT_VERDATA,
    UOFT_MUL,
    UOFT_UOP
};

enum UOFILE_IDS {
    UOF_STATIC,
    UOF_STAIDX,
    UOF_MAP,
    UOF_TILEDATA,
    UOF_MULTI_IDX,
    UOF_MULTI_MUL
};


extern class UOFileReader : private std::ifstream {
private:
    std::string file_path;  ///< path for the files. TODO: set it from .ini.
    std::streampos _size;   ///< size of the file, set on open.

    /**
    * @brief Retrieves the full name of the given file (type & id & format).
    *
    * @param file File type.
    * @param id Expansion id.
    * @param searchuop Search for .uop instead of .mul files.
    * @return the full name (eg map3LegacyMUL.uop)
    */
    std::string get_file_name(UOFILE_IDS file, t_ubyte id = 0, bool searchuop = true);
    /**
    * @brief Overrides seekg with custom checks.
    *
    * Checks if the index requested is within the file's size.
    * @param index Position to set the pointer at.
    * @param at std::ios::beg / end.
    * @return success or fail.
    */
    bool _seek(t_uqword index, t_ubyte at);
    /**
    * @brief Overrides read with custom checks.
    *
    * Checks if the retrieved data is valid (strlen == cnt).
    * @param data Stream receiving readed data.
    * @param cnt Amount of bytes to read.
    * @return success or fail.
    */
    bool _read(t_byte* data, t_uqword cnt);
    /**
    * @brief Overrides open with custom checks.
    *
    * Checks if the is a file already open, also sets _size for fast later accesses.
    * @param file Name of the file to be opened.
    * @param flags Flags used to open the file.
    * @return success or fail.
    */
    bool _open(const t_byte *file, t_udword flags = std::ios::in | std::ios::binary);

    /**
    * @brief Returns the stored size of this file.
    *
    * @return the size.
    */
    std::streampos get_size();
public:
    UOFileReader();
    ~UOFileReader();

    /**
    * @brief Reads mapx.mul/uop.
    *
    * Performs a full read of the map file(s) and insert their data into the given stream.
    * @param id ExpansionID of the file to read (0 = lbr, 2 = AOS, 3 = SE, 4 = ML, 5 = SA).
    */
    void read_map(t_ubyte id, UOMapPoint **&data);

} uofilereader;

#endif //_TORUS_GAME_UO_FILE_READER_H