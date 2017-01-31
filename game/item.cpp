#include "item.h"
#include "../debug/callstack.h"

Item::Item() : Artifact(UID_ITEM) {
    ADDTOCALLSTACK();
}

Item::Item(t_udword uid) : Artifact(uid) {
    ADDTOCALLSTACK();
}

Item::~Item(){
    ADDTOCALLSTACK();
}

bool Item::can_move(){
    ADDTOCALLSTACK();
    if (has_flag(IFLAG_LOCKED | IFLAG_STATIC)) {
        return false;
    }
    return true;
}
