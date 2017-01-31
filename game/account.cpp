
#include "../network/socket.h"
#include "../debug/callstack.h"
#include "account.h"
#include "char.h"

Account::Account(const t_byte *name, const t_byte *pw, t_udword flags , t_udword exp ){
    ADDTOCALLSTACK();
}

Account::~Account(){
    ADDTOCALLSTACK();
    for (t_byte i = 0; i < 7; i++) {
        if (_charlist[i]) {
            delete _charlist[i];
            _charlist[i] = 0;
        }
    }
}

t_byte Account::get_char_count(){
    ADDTOCALLSTACK();
    t_byte count = 0;
    for (t_byte i = 0; i < 7; i++) {
        if (_charlist[i])
            count++;
    }
    return count;
}

t_byte Account::get_max_chars(){
    ADDTOCALLSTACK();
    if (_expansion & EXP_SA)
        return 7;
    return 5;
    // TODO: proper expansion checks to retrieve the maximum amount of characters for this account.
}

bool Account::can_add_char(){
    ADDTOCALLSTACK();
    if (get_char_count() >= get_max_chars())
        return false;
    return true;
}

bool Account::add_char(Char *chr){
    ADDTOCALLSTACK();
    for (t_byte i = 0; i < 7; i++) {
        if (_charlist[i])
            continue;
        _charlist[i] = chr;
        return true;
    }
    return false;
}

bool Account::delete_char(Char *chr){
    ADDTOCALLSTACK();
    for (t_byte i = 0; i < 7; i++) {
        if (_charlist[i] == chr) {
            delete _charlist[i];
            _charlist[i] = 0;
            fix_charlist();
            return true;
        }
    }
    return false;
}

void Account::fix_charlist(){
    ADDTOCALLSTACK();
    for (t_byte i = 0; i < 7; i++) {
        if (!_charlist[i] && _charlist[i+1]) {
            _charlist[i] = _charlist[i+1];
            _charlist[i+1] = 0;
        }
    }
}

void Account::connect(Socket * socket){
    ADDTOCALLSTACK();
    _socket = socket;
    _lastip = _socket->get_ip();
}
