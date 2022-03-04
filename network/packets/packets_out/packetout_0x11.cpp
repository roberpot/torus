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

#include <game/char.h>
#include <game/uo_files/map_list.h>
#include <network/packets/packetlist.h>
#include <debug_support/info.h>


void PacketOut_0x11::set_data(Char* character)
{
    write_udword(character->get_uid().get_uid());
    write_string(character->get_name(), 30);
    write_word(character->get_stat(StatType::HITS).get_base());
    write_word(character->get_stat(StatType::HITS).get_max());
    write_byte(0);  //TODO: Name change for pets.
    t_byte features = 02|03|04|05|06;   //TODO: Client features (0x02 = T2A attributes, 0x03 = Renaissance attributes, 0x04 = AOS attributes, 0x05 = ML attributes, 0x06 = KR attributes).
    write_byte(features);
    write_byte(t_byte(character->get_gender()));
    write_uword(character->get_stat(StatType::STR).get_base()); //TODO: Re-check if client accepts unsigned values for stats
    write_uword(character->get_stat(StatType::DEX).get_base());
    write_uword(character->get_stat(StatType::INT).get_base());
    write_uword(character->get_stat(StatType::STAM).get_base());
    write_uword(character->get_stat(StatType::STAM).get_max());
    write_uword(character->get_stat(StatType::MANA).get_base());
    write_uword(character->get_stat(StatType::MANA).get_max());
    write_dword(0);//TODO: Gold count.
    write_word(0);//TODO: Armor count/Resphysical for AOS features.
    write_uword(character->get_stat(StatType::WEIGHT).get_base());
    write_uword(character->get_stat(StatType::WEIGHT).get_max());
    write_byte(t_byte(character->get_race()));
    //TODO: Send each stat by the features enabled:
    //T2A
    write_word(300);//TODO: Statcap
    //Renaissance
    write_byte(5);  //TODO: Followers.
    write_byte(5);  //TODO: MaxFollowers.
    //AOS
    write_uword(character->get_stat(StatType::RESFIRE).get_base());
    write_uword(character->get_stat(StatType::RESCOLD).get_base());
    write_uword(character->get_stat(StatType::RESPOISON).get_base());
    write_uword(character->get_stat(StatType::RESENERGY).get_base());
    write_uword(0);//TODO: Luck
    write_uword(0);//TODO: WeaponDmgMin
    write_uword(0);//TODO: WeaponDmgMax
    write_udword(0);//TODO: Tithing
    //KR
    /*
     * TODO:KR properties:
        word	Hit Chance Increase(KR attribute)
        word	Swing Speed Increase(KR attribute)
        word	Damage Chance Increase(KR attribute)
        word	Lower Reagent Cost(KR attribute)
        word	Hit Points Regeneration(KR attribute)
        word	Stamina Regeneration(KR attribute)
        word	Mana Regeneration(KR attribute)
        word	Reflect Physical Damage(KR attribute)
        word	Enhance Potions(KR attribute)
        word	Defense Chance Increase(KR attribute)
        word	Spell Damage Increase(KR attribute)
        word	Faster Cast Recovery(KR attribute)
        word	Faster Casting(KR attribute)
        word	Lower Mana Cost(KR attribute)
        word	Strength Increase(KR attribute)
        word	Dexterity Increase(KR attribute)
        word	Intelligence Increase(KR attribute)
        word	Hit Points Increase(KR attribute)
        word	Stamina Increase(KR attribute)
        word	Mana Increase(KR attribute)
        word	Maximum Hit Points Increase(KR attribute)
        word	Maximum Stamina Increase(KR attribute)
        word	Maximum Mana Increase(KR attribute)
    */
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
    write_uword(0);
}

/*
dword	Serial
char[30]	Name
word	Hit Points
word	Maximum Hit Points
byte	Allow Name Change (0x01 = yes, 0x00 = no)
byte	Supported Features (0x02 = T2A attributes, 0x03 = Renaissance attributes, 0x04 = AOS attributes, 0x05 = ML attributes, 0x06 = KR attributes)
byte	Gender
word	Strength
word	Dexterity
word	Intelligence
word	Stamina
word	Maximum Stamina
word	Mana
word	Maximum Mana
dword	Gold
word	Armor Rating (Physical Resistance since AOS)
word	Weight
word	Maximum Weight (ML attribute)
byte	Race (0x01 = Human, 0x02 = Elf, 0x03 = Gargoyle) (ML attribute)
word	Stat Cap (T2A attribute)
byte	Followers (Renaissance attribute)
byte	Maximum Followers (Renaissance attribute)
word	Fire Resistance (AOS attribute)
word	Cold Resistance (AOS attribute)
word	Poison Resistance (AOS attribute)
word	Energy Resistance (AOS attribute)
word	Luck (AOS attribute)
word	Minimum Weapon Damage (AOS attribute)
word	Maximum Weapon Damage (AOS attribute)
dword	Tithing Points (AOS attribute)
word	Hit Chance Increase (KR attribute)
word	Swing Speed Increase (KR attribute)
word	Damage Chance Increase (KR attribute)
word	Lower Reagent Cost (KR attribute)
word	Hit Points Regeneration (KR attribute)
word	Stamina Regeneration (KR attribute)
word	Mana Regeneration (KR attribute)
word	Reflect Physical Damage (KR attribute)
word	Enhance Potions (KR attribute)
word	Defense Chance Increase (KR attribute)
word	Spell Damage Increase (KR attribute)
word	Faster Cast Recovery (KR attribute)
word	Faster Casting (KR attribute)
word	Lower Mana Cost (KR attribute)
word	Strength Increase (KR attribute)
word	Dexterity Increase (KR attribute)
word	Intelligence Increase (KR attribute)
word	Hit Points Increase (KR attribute)
word	Stamina Increase (KR attribute)
word	Mana Increase (KR attribute)
word	Maximum Hit Points Increase (KR attribute)
word	Maximum Stamina Increase (KR attribute)
word	Maximum Mana Increase (KR attribute)
*/





