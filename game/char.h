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

#ifndef __TORUS_GAME_CHAR_H_
#define __TORUS_GAME_CHAR_H_

#include <library/types.h>
#include <game/artifact.h>
#include <game/enums.h>
#include <game/chars/char_props.h>
#include <game/chars/char_stats.h>

// Movement flags
#define CFLAG_CAN_MOVE          0x000001    // Can walk.
#define CFLAG_CAN_RUN           0x000002    // Can run.
#define CFLAG_CAN_FLY           0x000004    // Flying NPCs / Playable Gargoyles actively flying.
#define CFLAG_CAN_SWIN          0x000008    // Swim (move on water).
#define CFLAG_WAR               0x000010    // War mode

// Gearing flags
#define CFLAG_CAN_USE_HANDS     0x000010    // Can use items / Equip in hands.
#define CFLAG_CAN_EQUIP         0x000020    // Can equip items (armor, clothes... memories and internal items excluded).

// Misc?
#define CFLAG_HIDDEN            0x001000    // Hidding skill / Invis pot / spell.
#define CFLAG_POISONED          0x002000    // Poisoned

//GM Flags
#define CFLAG_GM                0x010000    // GM ON.
#define CFLAG_INVIS             0x020000    // GM invis.
#define CFLAG_INVUL             0x040000    // Invulnerable

#define DISTANCE_SEE_CHARS 14
#define DISTANCE_SEE_ITEMS 14
#define DISTANCE_SEE_MULTIS 18



class Account;
class Item;
class Client;

class Char : 
    public Artifact,
    public CharProps {
private:
    CharStats _stats[int(StatType::STATS_QTY)];
    Client* _client;         ///< Client attached to this char.
    BodyType _body;
    ~Char();

public:
    Char();
    Char(udword_t uid);

// Base & Stats
    CharStats &get_stat(StatType key);
    bool can_move() override;
    void remove();
    bool tick();
    bool can_see(Char *target);
    bool can_see(Item *target);

//Account&Client
private:
    Account *_account;
public:
    Account *get_account();
    void set_account(Account *account);
    void attach_client(Client* client);
    void detatch_client();
    Client* get_client();


public:
    uword_t get_status_flag(Char *viewer);
    bool can_equip(udword_t iflags);
    BodyType get_body();
    void set_body(BodyType body);
};

#endif // __TORUS_GAME_CHAR_H_