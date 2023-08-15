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

#include <game/item.h>
#include <debug_support/callstack.h>
#include <game/server.h>

Item::Item() : Artifact(UID_ITEM) {
  ADDTOCALLSTACK();
  _id = ItemId::FORGE;
}

Item::Item(const Item& other) {
}

Item::Item(udword_t uid) : Artifact(uid) {
  ADDTOCALLSTACK();
  _id = ItemId::FORGE;
}

Item::~Item() {
  ADDTOCALLSTACK();
}

ItemId Item::get_id() const {
  return _id;
}

void Item::set_id(const ItemId& id) {
  _id = id;
}

bool Item::can_move() {
  ADDTOCALLSTACK();
  if (has_flag(IFLAG_LOCKED | IFLAG_STATIC)) {
    return false;
  }
  return true;
}

void Item::remove() {
  ADDTOCALLSTACK();
  server.del_item(this);
}

bool Item::tick() {
  ADDTOCALLSTACK();
  return true;
}
