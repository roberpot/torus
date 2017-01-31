#ifndef _TORUS_GAME_ARTIFACT_H
#define _TORUS_GAME_ARTIFACT_H

#include "../core/types.h"
#include "uid.h"
#include <combaseapi.h>

struct Pos {
    t_word x = 1;
    t_word y = 1;
    t_byte z = 0;
    t_ubyte map = 0;
    bool can_move_to_coord(t_word destX, t_word destY);
    bool can_move_to_z(t_byte destZ);
    bool can_move_to_map(t_ubyte destMap);
    bool can_move_to(t_word destX, t_word destY, t_byte destZ, t_ubyte destMap);
};

class Char;
class Item;

class Artifact : protected Uid, public Pos {
private:
    const t_byte *_name;
    Pos _pos;
    t_uword _flags;
protected:
    //Uid _uid;
    virtual ~Artifact();
    Artifact(t_udword uid);
    Char *get_char();
    Item *get_item();
public:
    //Name
    const t_byte *get_name();
    void set_name(const t_byte *name);

    //Pos
    void move_to(t_word destX, t_word destY);
    void set_z(t_byte destZ);
    void set_map(t_ubyte destMap);
    void set_pos(t_word destX, t_word destY, t_byte destZ, t_ubyte destMap);
    virtual bool can_move() = 0;

    //Flags
    bool has_flag(t_udword flag);
    void set_flag(t_udword flag);
    void unset_flag(t_udword flag);
    void switch_flag(t_udword flag);
};

#endif //_TORUS_GAME_ARTIFACT_H