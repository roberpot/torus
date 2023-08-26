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

#ifndef __TORUS_GAME_UIDM_H
#define __TORUS_GAME_UIDM_H

#include <map>
#include <queue>
#include <library/types.h>
#include <game/uid.h>
#include <game/world_object_container.h>

class UidStorage {
  WorldUidsContainer _uids;

  std::queue<udword_t> _free_uids;
  udword_t _highest_uid;

 public:
  UidStorage();

  /**
   * @brief Load a given UID, called when loading the world.
   * @param uid the UID to load.
   * @return True if the UID doesn't exist already, false otherwise.
   */
  bool load(const Uid& uid);

  /**
   * @brief Called when the world has finished loading.
   * This will loop from 1 to the highest_uid to find free uids,
   * these uids will be stored in _free_uids and will be used instead
   * of creating new uids.
   */
  void post_load();

  /**
   * @brief Create a new UID of the given type.
   * @param mask The type.
   * @return the new uid.
   */
  Uid create(UidMask mask);

  /**
   * @brief Removes the given uid from the current used ones.
   * It also adds it to the _free_uids queue.
   * @param uid The Uid to remove.
   */
  void remove(const Uid& uid);

  /**
   * @brief Server tick.
   */
  void tick();
};

extern class UidManager {
  UidStorage _char_uids;
  UidStorage _item_uids;
  UidStorage _account_uids;
  UidStorage _resource_uids;
  UidStorage _undefined_uids;

 public:
  UidManager();

  /**
   * @brief Called when the world has finished loading.
   * Calls post_load() in all of the internal storages.
   */
  void post_load();

  /**
   * @brief Load a given UID, called when loading the world.
   * The Uid is being loaded in it's related storage by reading
   * first the Uid's Mask.
   * @param uid the UID to load.
   * @return True if the UID doesn't exist already, false otherwise.
   */
  void load(const Uid& uid);

  /**
   * @brief Create the Uid in the related storage, given in the <b>type</b> param.
   * @param type The type of Uid
   * @return the new uid.
   */
  Uid create(UidMask type);

  /**
   * @brief Removes the given Uid from it's storage.
   * @param uid The Uid to remove.
   */
  void remove(const Uid& uid);

  /**
   * @brief Server tick.
   */
  void tick();

} uidmanager;


#endif  // __TORUS_GAME_UIDM_H