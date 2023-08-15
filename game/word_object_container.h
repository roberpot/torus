#ifndef __TORUS_GAME_WOC_H
#define __TORUS_GAME_WOC_H

#include <library/stack.h>
#include <map>
#include <string>
#include <game/char.h>
#include <game/item.h>
#include <game/uid.h>
#include <threads/mutex.h>


/**
 * Compare for std::strings.
 */
class std_string_compare {
 public:
  bool operator()(const std::string& a, const std::string& b) const {
    return a.compare(b) < 0;
  }
};

/*template <typename O>
class WorldidContainer : public WorldContainer<id, O> {};*/

/*template <typename O>
class WorldStringContainer : public WorldContainer<std::string, O, std_string_compare>{};*/


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
          class Compare   = std::less<Uid>,
          class Allocator = std::allocator<std::pair<const O, T>>>
class WorldContainer {
  std::map<T, O, Compare, Allocator> _container;  // TODO: Use ttl map to have this container mutex protected.
  ttl::dynamicstack<T> _delete_queue;

 public:
  void add(const T& id, O object);
  bool has(const T& id);
  O get(const T& id);
  void remove(const T& id);
  void delete_now(const T& id);
  void delete_later(const T& id);
  void delete_all();
  void clear();
  size_t size();
  void tick();
};


template <class T, class O, class Compare, class Allocator>
void WorldContainer<T, O, Compare, Allocator>::add(const T& id, O object) {
  bool can_add = true;
  if (!_container.empty()) {
    if (_container.find(id) != _container.end()) {
      // err?
      can_add = false;
    }
  }

  if (can_add) {
    _container[id] = object;
  }
}

template <class T, class O, class Compare, class Allocator>
bool WorldContainer<T, O, Compare, Allocator>::has(const T& id) {
  return _container.find(id) != _container.end();
}

template <class T, class O, class Compare, class Allocator>
O WorldContainer<T, O, Compare, Allocator>::get(const T& id) {
  O ret   = nullptr;
  auto it = _container.find(id);
  if (it != _container.end()) {
    ret = it->second;
  }
  return ret;
}

template <class T, class O, class Compare, class Allocator>
void WorldContainer<T, O, Compare, Allocator>::remove(const T& id) {
  auto it = _container.find(id);
  if (it != _container.end()) {
    _container.erase(it);
  }
}

template <class T, class O, class Compare, class Allocator>
void WorldContainer<T, O, Compare, Allocator>::delete_now(const T& id) {
  T t = get(id);
  if (t) {
    delete t;
    remove(id);
  }
}

template <class T, class O, class Compare, class Allocator>
void WorldContainer<T, O, Compare, Allocator>::delete_later(const T& id) {
  // TODO object->delete_later(); // Clear timers, references, etc
  _delete_queue.push(id);
}

template <class T, class O, class Compare, class Allocator>
void WorldContainer<T, O, Compare, Allocator>::delete_all() {
  for (auto& it : _container) {
    delete (*it)->second;
  }
  _container.clear();
}

template <class T, class O, class Compare, class Allocator>
void WorldContainer<T, O, Compare, Allocator>::clear() {
  _container.clear();
}

template <class T, class O, class Compare, class Allocator>
size_t WorldContainer<T, O, Compare, Allocator>::size() {
  return _container.size();
}

template <class T, class O, class Compare, class Allocator>
void WorldContainer<T, O, Compare, Allocator>::tick() {
  if (!_delete_queue.empty()) {
    while (!_delete_queue.empty()) {
      id id = _delete_queue.top();
      _delete_queue.pop();
      delete _container[id];
      _container.erase(id);
    }
  }
}

class Account;

using WorldCharactersContainer = WorldContainer<Uid, Char*>;
using WorldItemsContainer      = WorldContainer<Uid, Item*>;
using WorldAccountsContainer   = WorldContainer<Uid, Account*>;
using ServerAccountsContainer  = WorldContainer<std::string, Account*, std_string_compare>;

#endif  // __TORUS_GAME_WOC_H