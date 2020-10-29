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

#ifndef __TORUS_GAME_CHAR_STATS_H_
#define __TORUS_GAME_CHAR_STATS_H_

#include <library/types.h>

enum STAT_TYPE {
    STAT_STR,
    STAT_DEX,
    STAT_INT,
    PRIMARY_STAT_QTY,   ///< Main stat type, can have _base and _mod 

    STAT_HITS = PRIMARY_STAT_QTY,
    STAT_STAM,
    STAT_MANA,
    SECONDARY_STAT_QTY, ///< Secondary stat type, can have _base, _mod, _max (doesn't modify anything, it's an standalone prop), _regen_delay, _regen_val and _last_regen
    STATS_QTY = SECONDARY_STAT_QTY,

    STAT_RESFIRE = SECONDARY_STAT_QTY,
    STAT_RESCOLD,
    STAT_RESENERGY,
    STAT_RESPOISON,
    STAT_RESPHYS,
    RESISTS_START = STAT_RESFIRE,
    RESISTS_QTY = STAT_RESPHYS, ///< Resistances, can have _base, _mod and _max (modifies _base).

    STAT_WEIGHT ///< Weight, can have _base (current weight), _max (max carryable weight), _mod (modifies _max (ModMaxWeight prop)).
};

class CharStats {
public:
    CharStats();
    ~CharStats();
private:
    // Common stat props
    uword_t _base;  ///< base, real val.
    uword_t _mod;   ///< modification over base.
    uword_t _max;   ///< max value.
// Regen props (only for hits, mana and stam).
    uword_t _regen_delay;   ///< ticks between regens.
    uword_t _regen_val;     ///< amount to regen.
    uqword_t _last_regen;   ///< tick in which last regen was done.
public:
// Common
    void set_base(uword_t val);
    uword_t get_base();
    void set_mod(uword_t val);
    uword_t get_mod();
    void set_max(uword_t val);
    uword_t get_max();
    void set_val(uword_t base, uword_t mod);
    uword_t get_val();
// Regens
    void set_regen_delay(uword_t val);
    uword_t get_regen_delay();
    void set_regen_val(uword_t val);
    uword_t get_regen_val();
    void regen();
};

#endif // __TORUS_GAME_CHAR_STATS_H_