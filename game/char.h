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
#include "chars/char_props.h"
#include "chars/char_stats.h"

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


class Char : 
    public Artifact,
    public CharProps {
private:
    CharStats _stats[STATS_QTY];
public:
    Char();
    Char(t_udword uid);
    ~Char();
    CharStats &get_stat(STAT_TYPE key);
    bool can_move() override;
};

#endif //_TORUS_GAME_CHAR_H_