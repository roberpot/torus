#include "word_object_container.h"

#include <game/item.h>
#include <game/char.h>
#include <game/account.h>
#include <game/uid.h>


template <class T, class O, class Compare, class Allocator>
bool WorldContainer<T, O, Compare, Allocator>::has(const T& id) const{
  return _container.find(id) != _container.end();
}

template <class T, class O, class Compare, class Allocator>
O WorldContainer<T, O, Compare, Allocator>::get(const T& id) const {
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
size_t WorldContainer<T, O, Compare, Allocator>::size() const {
  return _container.size();
}

template <class T, class O, class Compare, class Allocator>
void WorldContainer<T, O, Compare, Allocator>::tick() {
  if (!_delete_queue.empty()) {
    while (!_delete_queue.empty()) {
      Uid id = _delete_queue.top();
      _delete_queue.pop();
      delete _container[id];
      _container.erase(id);
    }
  }
}