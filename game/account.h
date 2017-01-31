#ifndef _TORUS_GAME_ACCOUNT_H
#define _TORUS_GAME_ACCOUNT_H

#include "../core/types.h"

class Char;
class Socket;

// Expansion Flags
#define EXP_LBR 0x001   ///< Default game.
#define EXP_T2A 0x002   ///< The Second Age.
#define EXP_AOS 0x004   ///< Age Of Shadows.
#define EXP_SE  0x008   ///< Samurai Empire.
#define EXP_ML  0x010   ///< Mondain's Legacy.
#define EXP_SA  0x020   ///< Stygian Abyss.
#define EXP_HS  0x040   ///< High Seas.

class Account {
private:
    const t_byte *_name;
    const t_byte *_password;
    Char *_charlist[7];
    t_udword _flags;
    t_uword _expansion;
    const t_byte *_lastip;
    Socket *_socket;
public:
    Account(const t_byte *name, const t_byte *pw, t_udword flags = 0, t_udword exp = 0);
    ~Account();

    t_byte get_char_count();
    t_byte get_max_chars();
    bool can_add_char();
    bool add_char(Char *chr);
    bool delete_char(Char *chr);
    void fix_charlist();
    void connect(Socket *socket);
};

#endif //_TORUS_GAME_ACCOUNT_H