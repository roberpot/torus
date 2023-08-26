
#include "uid_manager.h"
#include <shell.h>

UidStorage::UidStorage() : _highest_uid(0) {
}

bool UidStorage::load(const Uid& uid) {
  udword_t real_uid = uid.get_uid_base();
  bool added        = false;
  if (_uids.has(real_uid)){
    _uids.add(real_uid, uid);
    added = true;
  } else {
    TORUSSHELLECHO("Error loading Uid " << uid.get_uid() << ": it is already loaded.");
  }
  return added;
}

void UidStorage::post_load() {
  for (udword_t i = 1; i <= _highest_uid; ++i) {
    if (!_uids.has(i)) {
      _free_uids.push(i);
    }
  }
}

Uid UidStorage::create(UidMask mask) {
  udword_t uid = 0;
  if (!_free_uids.empty()) {
    uid = _free_uids.front();
    _free_uids.pop();
  } else {
    uid = ++_highest_uid;
  }
  Uid new_uid(uid);
  if (!_uids.has(uid)) {
    _uids.add(uid, new_uid);
  } else {
    // safety check, should never happen.
    new_uid = create(mask);
  }
  return new_uid;
}

void UidStorage::remove(const Uid& uid) {
  _free_uids.push(uid.get_uid_base());
  _uids.delete_later(uid.get_uid_base());
}

void UidStorage::tick() {
    _uids.tick();
}

UidManager::UidManager() {
}

void UidManager::post_load() {
  _char_uids.post_load();
  _item_uids.post_load();
  _account_uids.post_load();
  _resource_uids.post_load();
  _undefined_uids.post_load();
}

void UidManager::load(const Uid& uid) {
  UidMask mask = uid.get_mask();
  switch (mask) {
    case UidMask::UID_CHAR:
      _char_uids.load(uid);
      break;
    case UidMask::UID_ITEM:
      _item_uids.load(uid);
      break;
    case UidMask::UID_ACCOUNT:
      _account_uids.load(uid);
      break;
    case UidMask::UID_RESOURCE:
      _resource_uids.load(uid);
      break;
    default:
      _undefined_uids.load(uid);
      break;
  }
}

Uid UidManager::create(UidMask type) {
  Uid uid;
  switch (type) {
    case UidMask::UID_CHAR:
      uid = _char_uids.create(type);
      break;
    case UidMask::UID_ITEM:
      uid = _item_uids.create(type);
      break;
    case UidMask::UID_ACCOUNT:
      uid = _account_uids.create(type);
      break;
    case UidMask::UID_RESOURCE:
      uid = _resource_uids.create(type);
      break;
    default:
      uid = _undefined_uids.create(type);
      break;
  }
  return uid;
}

void UidManager::remove(const Uid& uid) {
  UidMask mask = uid.get_mask();
  switch (mask) {
    case UidMask::UID_CHAR:
      _char_uids.remove(uid);
      break;
    case UidMask::UID_ITEM:
      _item_uids.remove(uid);
      break;
    case UidMask::UID_ACCOUNT:
      _account_uids.remove(uid);
      break;
    case UidMask::UID_RESOURCE:
      _resource_uids.remove(uid);
      break;
    default:
      _undefined_uids.remove(uid);
      break;
  }
}

void UidManager::tick() {
  _char_uids.tick();
  _item_uids.tick();
  _account_uids.tick();
  _resource_uids.tick();
  _undefined_uids.tick();
}
