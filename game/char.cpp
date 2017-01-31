#include "char.h"
#include "../debug/callstack.h"

Char::Char() : Artifact(UID_CHAR){
    ADDTOCALLSTACK();
}

Char::Char(t_udword uid) : Artifact( uid) {
    ADDTOCALLSTACK();
}

Char::~Char(){
    ADDTOCALLSTACK();
}

Char::Stat &Char::get_stat(STAT_TYPE key){
    ADDTOCALLSTACK();
    return _stats[key];
}

bool Char::can_move(){
    ADDTOCALLSTACK();
    if (has_flag(CFLAG_GM)) {
        return true;
    }
    if (!has_flag(CFLAG_CAN_MOVE)) {
        return false;
    }
    return true;
}

void Char::Stat::set_base(t_uword val){
    ADDTOCALLSTACK();
    _base = val;
}

void Char::Stat::set_mod(t_uword val){
    ADDTOCALLSTACK();
    _mod = val;
}

void Char::Stat::set_max(t_uword val){
    ADDTOCALLSTACK();
    _max = val;
}

void Char::Stat::set_val(t_uword base, t_uword mod){
    ADDTOCALLSTACK();
    _base = base;
    _mod = mod;
}

t_uword Char::Stat::get_base(){
    ADDTOCALLSTACK();
    return _base;
}

t_uword Char::Stat::get_mod(){
    ADDTOCALLSTACK();
    return _mod;
}

t_uword Char::Stat::get_max(){
    ADDTOCALLSTACK();
    return _max;
}

t_uword Char::Stat::get_val(){
    ADDTOCALLSTACK();
    return _base + _mod;
}