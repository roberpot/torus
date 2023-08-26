/*
 * This file is part of Torus.
 * Torus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, O, boolr
 * (at your option) any later version.
 * Torus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with Torus. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TORUS_GAME_WOC_H
#define __TORUS_GAME_WOC_H

#include <library/stack.h>
#include <map>
#include <string>
#include <game/char.h>
#include <game/item.h>
#include <game/uid.h>
#include <threads/mutex.h>
#include <type_traits>


/**
 * Compare for std::strings.
 */
class std_string_compare {
 public:
  bool operator()(const std::string& a, const std::string& b) const {
    return a.compare(b) < 0;
  }
};

template <typename V, bool is_pointer = std::is_pointer<V>::value>
class Destructor {
 public:
  void destroy(V& v) {
    (void)v;
  }
};

template <typename V>
class Destructor<V, true> {
 public:
  void destroy(V& v) {
    delete v;
  }
};

/**
* This class is used to store and retrieve the world objects (characters, items ...).
* The argument T refers to the type of identificator used to store the objects,
* Eg: id for chars, std::string for accounts...
* The argument O refers to the type of the object to store, Eg: Char*, Account ...
*
* TODO MT:  -add must be delayed until the end of the tick.
*           -delete_now must be private and called at the end of the tick.
*   This will prevent writes out of time, for example if you do any change to a character,
    which is flagged to be deleted, the change will take effect and so it will be deleted later
    but doing it this way should be error-safe.
*/
template <typename T,
          typename O,
          class compare    = std::less<T>,
          class allocator  = std::allocator<std::pair<const T, O>>,
          class destructor = Destructor<O>>
class WorldContainer {
 public:
  using KeyType    = T;
  using ValueType  = O;
  using Compare    = compare;
  using Allocator  = allocator;
  using Destructor = destructor;

  /**
   * @brief Enqueue an object to add later in tick().
   * @param id The key of the object.
   * @param object The object to add later.
   */
  void add(const T& id, const O& object);

  /**
   * @brief Checks if the given key already exists.
   * @param id The key to check.
   * @return True if the key is present, false otherwise.
   */
  bool has(const T& id) const;

  /**
   * @brief Get the ValueType with the given KeyType.
   * @param id the KeyType.
   * @return the ValueType.
   */
  O& get(const T& id);

  /**
   * @brief
   * @param id
   * @return
   */
  bool delete_now(const T& id);

  /**
   * @brief Enqueue an object to be removed later in the tick().
   * @param id The key of the object to remove.
   * @return True if the object has been queued.
   */
  void delete_later(const T& id);

  /**
   * @brief Delete all the objects stored.
   * Warning, this will really delete the objects, really! It should be called
   * only on shutdown, use at own risk otherwise!
   */
  void delete_all();

  /**
   * @brief Clear the container, but does not remove the objects stored if the ValueType is a pointer.
   */
  void clear();

  /**
   * @brief Retrieve the count of entries stored.
   * @return the count.
  */
  size_t size() const;

  /**
   * @brief Extension of mainloop.
   * This method does update the container, removing the entries that were
   * flagged to be deleted then adding the new ones.
  */
  void tick();

  /**
   * @brief Map overload for [] operator.
   * @param id The KeyType for the ValueType to retrieve.
   * @return The ValueType, if found.
  */
  O& operator[](const T& id);

 private:
  std::map<T, O, Compare, Allocator> _container;  // TODO: Use ttl map to have this container mutex protected.
  ttl::dynamicstack<T> _delete_queue;
  std::map<T, O> _add_queue;

  void remove_value(O value);
};

template <class T, class O, class compare, class allocator, class destructor>
void WorldContainer<T, O, compare, allocator, destructor>::add(const T& id, const O& object) {
  bool can_add = true;
  if (!_container.empty()) {
    if (_container.find(id) != _container.end()) {
      // err?
      can_add = false;
    }
  }

  if (can_add) {
    _add_queue[id] = object;
  }
}

template <class T, class O, class compare, class allocator, class destructor>
bool WorldContainer<T, O, compare, allocator, destructor>::has(const T& id) const {
  return _container.find(id) != _container.end();
}

template <class T, class O, class compare, class allocator, class destructor>
O& WorldContainer<T, O, compare, allocator, destructor>::get(const T& id) {
  return _container.at(id);
}

template <class T, class O, class compare, class allocator, class destructor>
bool WorldContainer<T, O, compare, allocator, destructor>::delete_now(const T& id) {
  bool deleted = false;
  auto it      = _container.find(id);
  if (id != _container.end()) {
    remove_value(it.second);
    _container.erase(it.first);
    deleted = true;
  }
  return deleted;
}

template <class T, class O, class compare, class allocator, class destructor>
void WorldContainer<T, O, compare, allocator, destructor>::delete_later(const T& id) {
  // TODO object->delete_later(); // Clear timers, references, etc
  _delete_queue.push(id);
}

template <class T, class O, class compare, class allocator, class destructor>
void WorldContainer<T, O, compare, allocator, destructor>::delete_all() {
  for (auto& it : _container) {
    delete it.second;
  }
  _container.clear();
}

template <class T, class O, class compare, class allocator, class destructor>
void WorldContainer<T, O, compare, allocator, destructor>::clear() {
  _container.clear();
}

template <class T, class O, class compare, class allocator, class destructor>
size_t WorldContainer<T, O, compare, allocator, destructor>::size() const {
  return _container.size();
}

template <typename T, typename O, class compare, class allocator, class destructor>
inline O& WorldContainer<T, O, compare, allocator, destructor>::operator[](const T& id) {
  return _container[id];
}

template <typename T, typename O, class compare, class allocator, class destructor>
inline void WorldContainer<T, O, compare, allocator, destructor>::remove_value(O value) {
  Destructor d;
  d.destroy(value);
}

template <class T, class O, class compare, class allocator, class destructor>
void WorldContainer<T, O, compare, allocator, destructor>::tick() {
  if (!_delete_queue.empty()) {
    while (!_delete_queue.empty()) {
      KeyType id       = _delete_queue.top();
      ValueType object = _container[id];
      _delete_queue.pop();
      remove_value(object);
      _container.erase(id);
    }
  }

  if (!_add_queue.empty()) {
    for (auto& it : _add_queue) {
      KeyType uid      = it.first;
      ValueType object = it.second;
      if (_container.find(uid) == _container.end()) {
        _container[uid] = object;
      }
    }
    _add_queue.clear();
  }
}

template <typename T, typename O, class compare, class allocator, class destructor>
class WorldContainer<T, O*, compare, allocator, destructor> {

 public:
  using KeyType    = T;
  using ValueType  = O;
  using Compare    = compare;
  using Allocator  = allocator;
  using Destructor = destructor;

  /**
   * @brief Enqueue an object to add later in tick().
   * @param id The key of the object.
   * @param object The object to add later.
   */
  void add(const T& id, O* object);

  /**
   * @brief Checks if the given key already exists.
   * @param id The key to check.
   * @return True if the key is present, false otherwise.
   */
  bool has(const T& id) const;

  /**
   * @brief Get the ValueType with the given KeyType.
   * @param id the KeyType.
   * @return the ValueType.
   */
  O* get(const T& id);

  /**
   * @brief
   * @param id
   * @return
   */
  bool delete_now(const T& id);

  /**
   * @brief Enqueue an object to be removed later in the tick().
   * @param id The key of the object to remove.
   * @return True if the object has been queued.
   */
  void delete_later(const T& id);

  /**
   * @brief Delete all the objects stored.
   * Warning, this will really delete the objects, really! It should be called
   * only on shutdown, use at own risk otherwise!
   */
  void delete_all();

  /**
   * @brief Clear the container, but does not remove the objects stored if the ValueType is a pointer.
   */
  void clear();

  /**
   * @brief Retrieve the count of entries stored.
   * @return the count.
   */
  size_t size() const;

  /**
   * @brief Extension of mainloop.
   * This method does update the container, removing the entries that were
   * flagged to be deleted then adding the new ones.
   */
  void tick();

  /**
   * @brief Map overload for [] operator.
   * @param id The KeyType for the ValueType to retrieve.
   * @return The ValueType, if found.
   */
  O* operator[](const T& id);
  void remove_value(O* value);

 private:
  std::map<T, O*, Compare, Allocator> _container;  // TODO: Use ttl map to have this container mutex protected.
  ttl::dynamicstack<T> _delete_queue;
  std::map<T, O*> _add_queue;
};

template <typename T, typename O, class compare, class allocator, class destructor>
inline void WorldContainer<T, O*, compare, allocator, destructor>::add(const T& id, O* object) {
  bool can_add = true;
  if (!_container.empty()) {
    if (_container.find(id) != _container.end()) {
      // err?
      can_add = false;
    }
  }

  if (can_add) {
    _add_queue[id] = object;
  }
}

template <class T, class O, class compare, class allocator, class destructor>
bool WorldContainer<T, O*, compare, allocator, destructor>::has(const T& id) const {
  return _container.find(id) != _container.end();
}

template <class T, class O, class compare, class allocator, class destructor>
O* WorldContainer<T, O*, compare, allocator, destructor>::get(const T& id) {
  ValueType* value = nullptr;
  auto it = _container.find(id);
  if (it != _container.end()) {
    value = it->second;
  }
  return value;
}


template <typename T, typename O, class compare, class allocator, class destructor>
inline O* WorldContainer<T, O*, compare, allocator, destructor>::operator[](const T& id) {
  return _container[id];
}


template <class T, class O, class compare, class allocator, class destructor>
bool WorldContainer<T, O*, compare, allocator, destructor>::delete_now(const T& id) {
  bool deleted = false;
  auto it      = _container.find(id);
  if (id != _container.end()) {
    remove_value(it.second);
    _container.erase(it.first);
    deleted = true;
  }
  return deleted;
}

template <class T, class O, class compare, class allocator, class destructor>
void WorldContainer<T, O*, compare, allocator, destructor>::delete_later(const T& id) {
  // TODO object->delete_later(); // Clear timers, references, etc
  _delete_queue.push(id);
}

template <class T, class O, class compare, class allocator, class destructor>
void WorldContainer<T, O*, compare, allocator, destructor>::delete_all() {
  for (auto& it : _container) {
    delete it.second;
  }
  _container.clear();
}

template <class T, class O, class compare, class allocator, class destructor>
void WorldContainer<T, O*, compare, allocator, destructor>::clear() {
  _container.clear();
}

template <class T, class O, class compare, class allocator, class destructor>
size_t WorldContainer<T, O*, compare, allocator, destructor>::size() const {
  return _container.size();
}

template <class T, class O, class compare, class allocator, class destructor>
void WorldContainer<T, O*, compare, allocator, destructor>::tick() {
  if (!_delete_queue.empty()) {
    while (!_delete_queue.empty()) {
      KeyType id       = _delete_queue.top();
      ValueType* object = _container[id];
      _delete_queue.pop();
      remove_value(object);
      _container.erase(id);
    }
  }

  if (!_add_queue.empty()) {
    for (auto& it : _add_queue) {
      KeyType uid      = it.first;
      ValueType* object = it.second;
      if (_container.find(uid) == _container.end()) {
        _container[uid] = object;
      }
    }
    _add_queue.clear();
  }
}
template <typename T, typename O, class compare, class allocator, class destructor>
inline void WorldContainer<T, O*, compare, allocator, destructor>::remove_value(O* value) {
  delete value;
}

class Account;

using WorldCharactersContainer = WorldContainer<Uid, Char*>;
using WorldItemsContainer      = WorldContainer<Uid, Item*>;
using WorldAccountsContainer   = WorldContainer<Uid, Account*>;
using ServerAccountsContainer  = WorldContainer<std::string, Uid, std_string_compare>;
using WorldUidsContainer       = WorldContainer<udword_t, Uid>;

#endif  // __TORUS_GAME_WOC_H