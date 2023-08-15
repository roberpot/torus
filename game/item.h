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

#ifndef __TORUS_GAME_ITEM_H
#define __TORUS_GAME_ITEM_H

#include <library/types.h>
#include <game/artifact.h>
#include <game/enums.h>

#define UFLAG1_FLOOR		0x00000001  ///< 0= floor (Walkable at base position)
#define UFLAG1_EQUIP		0x00000002  ///< 1= equipable. m_layer is LAYER_TYPE
#define UFLAG1_NONBLOCKING	0x00000004  ///< 2= Signs and railings that do not block.
#define UFLAG1_LIQUID		0x00000008  ///< 3= blood,Wave,Dirt,webs,stain, (translucent items)
#define UFLAG1_WALL			0x00000010  ///< 4= wall type = wall/door/fireplace
#define UFLAG1_DAMAGE		0x00000020  ///< 5= damaging. (Sharp, hot or poisonous)
#define UFLAG1_BLOCK		0x00000040  ///< 6= blocked for normal human. (big and heavy)
#define UFLAG1_WATER		0x00000080  ///< 7= water/wet (blood/water)
#define UFLAG2_ZERO1		0x00000100  ///< 8= NOT USED (i checked)
#define UFLAG2_PLATFORM		0x00000200  ///< 9= platform/flat (can stand on, bed, floor, )
#define UFLAG2_CLIMBABLE	0x00000400  ///< a= climbable (stairs). m_height /= 2(For Stairs+Ladders)
#define UFLAG2_STACKABLE	0x00000800  ///< b= pileable/stackable (m_dwUnk7 = stack size ?)
#define UFLAG2_WINDOW		0x00001000  ///< c= window/arch/door can walk thru it
#define UFLAG2_WALL2		0x00002000  ///< d= another type of wall. (not sure why there are 2)
#define UFLAG2_A			0x00004000  ///< e= article a
#define UFLAG2_AN			0x00008000  ///< f= article an
#define UFLAG3_DESCRIPTION  0x00010000  ///< 10= descriptional tile. (Regions, professions, ...)
#define UFLAG3_TRANSPARENT	0x00020000  ///< 11= Transparent (Is used for leaves and sails)
#define UFLAG3_CLOTH		0x00040000  ///< 12= Probably dyeable ? effects the way the client colors the item. color gray scale stuff else must be converted to grayscale
#define UFLAG3_ZERO8		0x00080000  ///< 13= 0 NOT USED (i checked)
#define UFLAG3_MAP			0x00100000  ///< 14= map
#define UFLAG3_CONTAINER	0x00200000  ///< 15= container.
#define UFLAG3_EQUIP2		0x00400000  ///< 16= equipable (not sure why there are 2 of these)
#define UFLAG3_LIGHT		0x00800000  ///< 17= light source
#define UFLAG4_ANIM			0x01000000  ///< 18= animation with next several object frames.
#define UFLAG4_HOVEROVER	0x02000000  ///< 19= item can be hovered over (SA tiledata) (older tiledata has this applied to archway, easel, fountain - unknown purpose)
#define UFLAG4_WALL3		0x04000000  ///< 1a= tend to be types of walls ? I have no idea.
#define UFLAG4_BODYITEM		0x08000000  ///< 1b= Whole body item (ex.British", "Blackthorne", "GM Robe" and "Death shroud")
#define UFLAG4_ROOF			0x10000000  ///< 1c=
#define UFLAG4_DOOR			0x20000000  ///< 1d= door
#define UFLAG4_STAIRS		0x40000000  ///< 1e=
#define UFLAG4_WALKABLE		0x80000000  ///< 1f= We can walk here.
#define IFLAG_STATIC        0x00001000
#define IFLAG_LOCKED        0x00002000

#define CAN_EQUIP_ALL           0x000   ///< Everyone can equip.
#define CAN_EQUIP_MALE_ONLY     0x001   ///< Only male characters can equip.
#define CAN_EQUIP_FEMALE_ONLY   0x002   ///< Only female characters can equip.
#define CAN_EQUIP_HUMAN         0x004   ///< Only humans can equip.
#define CAN_EQUIP_ELF           0x008   ///< Only elves can equip.
#define CAN_EQUIP_GARGOYLE      0x010   ///< Only gargoyles can equip.
#define CAN_EQUIP_NONE          0x020   ///< Cannot be equiped.

class Item : public Artifact {
    ~Item();
    ItemId _id;
public:
    Item();
    Item(const Item& other);
    Item(udword_t uid);
    ItemId get_id() const;
    void set_id(const ItemId& id);
    bool can_move() override;
    void remove();
    bool tick();
};

#endif // __TORUS_GAME_ITEM_H
