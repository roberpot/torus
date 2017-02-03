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

#ifndef _TORUS_GAME_CHAR_H_
#define _TORUS_GAME_CHAR_H_

#include "../core/types.h"
#include "artifact.h"

// Movement flags
#define CFLAG_CAN_MOVE          0x000001    // Can walk.
#define CFLAG_CAN_RUN           0x000002    // Can run.
#define CFLAG_CAN_FLY           0x000004    // Flying NPCs / Playable Gargoyles actively flying.
#define CFLAG_CAN_SWIN          0x000008    // Swim (move on water).

// Gearing flags
#define CFLAG_CAN_USE_HANDS     0x000010    // Can use items / Equip in hands.
#define CFLAG_CAN_EQUIP         0x000020    // Can equip items (armor, clothes... memories and internal items excluded).

// Misc?
#define CFLAG_HIDDEN            0x001000    // Hidding skill / Invis pot / spell.
#define CFLAG_POISONED          0x002000    // Poisoned

//GM Flags
#define CFLAG_GM                0x010000    // GM ON.
#define CFLAG_INVIS             0x020000    // GM invis.

enum STAT_TYPE {
    STAT_STR,
    STAT_DEX,
    STAT_INT,
    PRIMARY_STAT_QTY,
    STAT_HITS = PRIMARY_STAT_QTY,
    STAT_STAM,
    STAT_MANA,
    SECONDARY_STAT_QTY
};

class Char : public Artifact {
private:
    struct Stat  {
    private:
        t_uword _base = 0;
        t_uword _mod = 0;
        t_uword _max = 0;
    public:
        Stat() {}
        ~Stat() {}
        void set_base(t_uword val);
        void set_mod(t_uword val);
        void set_max(t_uword val);
        void set_val(t_uword base, t_uword mod);
        t_uword get_base();
        t_uword get_mod();
        t_uword get_max();
        t_uword get_val();
    };
    Stat _stats[SECONDARY_STAT_QTY];
public:
    Char();
    Char(t_udword uid);
    ~Char();
    Stat &get_stat(STAT_TYPE key);
    bool can_move() override;
};

#endif //_TORUS_GAME_CHAR_H_