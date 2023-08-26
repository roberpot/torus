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

#include <game/uid.h>
#include <shell.h>
#include <debug_support/callstack.h>
#include <game/uid_manager.h>

Uid::Uid() {
  ADDTOCALLSTACK();
  _uid = static_cast<udword_t>(UidMask::UID_UNUSED);
}

Uid::Uid(udword_t uid) {
  ADDTOCALLSTACK();
  set_uid(uid);
}

Uid::Uid(const Uid& other) {
  ADDTOCALLSTACK();
  _uid = other._uid;
}

Uid::~Uid() {
  ADDTOCALLSTACK();
}

bool Uid::operator==(const Uid& uid) const {
  return _uid == uid._uid;
}
bool Uid::operator<(const Uid& uid) const {
  return _uid < uid._uid;
}

bool Uid::operator<=(const Uid& uid) const {
  return _uid <= uid._uid;
}

bool Uid::operator>(const Uid& uid) const {
  return _uid > uid._uid;
}

bool Uid::operator>=(const Uid& uid) const {
  return _uid >= uid._uid;
}

void Uid::set_uid(udword_t uid) {
  ADDTOCALLSTACK();
  _uid = uid;
}

void Uid::find_new_uid(UidMask mask) {
  ADDTOCALLSTACK();
  uidmanager.create(mask);
}

udword_t Uid::get_uid() const {
  return _uid;
}

udword_t Uid::get_uid_base() const {
  return _uid & ~(static_cast<udword_t>(UidMask::UID_CLEAR_MASK));
}

UidMask Uid::get_mask() const {
  return static_cast<UidMask>(_uid & ~(static_cast<udword_t>(UidMask::UID_INDEX_MASK)));
}

void Uid::set_uid_type(UidMask mask) {
  _uid |= static_cast<udword_t>(mask);
}

bool Uid::is_item() const {
  return _uid & UidMask::UID_ITEM;
}

bool Uid::is_resource() const {
  return _uid & UidMask::UID_RESOURCE;
}

bool Uid::is_char() const {
  return !is_item() && !is_resource();
}

bool Uid::is_valid() const {
  return ((_uid != static_cast<udword_t>(UidMask::UID_UNUSED)) && (_uid > 0));
}
