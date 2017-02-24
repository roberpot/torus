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

#include "../../debug/callstack.h"
#include "char_stats.h"

CharStats::CharStats() {
    _base = 0;
    _mod = 0;
    _max = 0;
    _regen_delay = 0;
    _regen_val = 0;
    _last_regen = 0;
}

CharStats::~CharStats() {
}

void CharStats::set_base(t_uword val) {
    ADDTOCALLSTACK();
    _base = val;
}

void CharStats::set_mod(t_uword val) {
    ADDTOCALLSTACK();
    _mod = val;
}

void CharStats::set_max(t_uword val) {
    ADDTOCALLSTACK();
    _max = val;
}

void CharStats::set_val(t_uword base, t_uword mod) {
    ADDTOCALLSTACK();
    _base = base;
    _mod = mod;
}

t_uword CharStats::get_base() {
    ADDTOCALLSTACK();
    return _base;
}

t_uword CharStats::get_mod() {
    ADDTOCALLSTACK();
    return _mod;
}

t_uword CharStats::get_max() {
    ADDTOCALLSTACK();
    return _max;
}

t_uword CharStats::get_val() {
    ADDTOCALLSTACK();
    return _base + _mod;
}

void CharStats::set_regen_delay(t_uword val) {
    ADDTOCALLSTACK();
    _regen_delay = val;
}

t_uword CharStats::get_regen_delay() {
    ADDTOCALLSTACK();
    return _regen_delay;
}

void CharStats::set_regen_val(t_uword val) {
    ADDTOCALLSTACK();
    _regen_val = val;
}

t_uword CharStats::get_regen_val() {
    ADDTOCALLSTACK();
    return _regen_val;
}

void CharStats::regen() {
    ADDTOCALLSTACK();
    if (_regen_delay == 0) // 0 = disabled
        return;
    t_uqword clock = 0;// TODO time control, smth like torustime.clock.get_current().
    t_uqword regen_diff = _last_regen - clock;
    if (regen_diff <= 0) {
        _base += _regen_val;
        _last_regen = clock;
    }
}
