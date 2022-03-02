#ifndef __TORUS_GAME_WOC_H
#define __TORUS_GAME_WOC_H

#include <library/stack.h>
#include <map>
#include <game/uid.h>
#include <threads/mutex.h>

class Char;

//#define WOCDef WorldObjectContainer<T, Compare, Allocator>
#define WOCSimple <WorldObjectContainer<T>

#define WOCTemp <class T, class Compare = std::less<Uid>, class Allocator = std::allocator<std::pair<const Uid, T>>>
#define WOCMethod <class T, class Compare, class Allocator>



template WOCTemp
class WorldObjectContainer
{
    std::map<Uid,T, Compare, Allocator> _container; //TODO: Use ttl map to have this container mutex protected.
    ttl::dynamicstack<Uid> _delete_queue;
public:
    void add(Uid uid, T object);
    bool has(Uid uid);
    bool has_uid(Uid uid);
    T get(Uid uid);
    void remove(Uid uid);
    void delete_now(Uid uid);
    void delete_later(Uid uid);
    void delete_all();
    void clear();
    size_t size();
    void tick();
};


template <class T, class Compare, class Allocator>
void WorldObjectContainer<T, Compare, Allocator>::add(Uid uid, T object)
{
    bool can_add = true;
    if (!_container.empty())
    {
        if (_container.find(uid) != _container.end())
        {
            //err?
            can_add = false;
        }
    }
    
    _container[uid] = object;
}

template <class T, class Compare, class Allocator>
bool WorldObjectContainer<T, Compare, Allocator>::has(Uid uid)
{
    return _container.find(object) != _container.end();
}

template <class T, class Compare, class Allocator>
bool WorldObjectContainer<T, Compare, Allocator>::has_uid(Uid uid)
{
    return _container.at(uid) != nullptr;
}

template <class T, class Compare, class Allocator>
T WorldObjectContainer<T, Compare, Allocator>::get(Uid uid)
{
    T ret = nullptr;
    auto it = _container.find(uid);
    if (it != _container.end())
    {
        ret = it->second;
    }
    return ret;
}

template <class T, class Compare, class Allocator>
void WorldObjectContainer<T, Compare, Allocator>::remove(Uid uid)
{
    auto it = _container.find(object);
    if (it != _container.end())
    {
        _container.erase(it);
    }
}

template <class T, class Compare, class Allocator>
void WorldObjectContainer<T, Compare, Allocator>::delete_now(Uid uid)
{
    T t = get(uid);
    if (t)
    {
        delete t;
        remove(uid);
    }
}

template <class T, class Compare, class Allocator>
void WorldObjectContainer<T, Compare, Allocator>::delete_later(Uid uid)
{
    //TODO object->delete_later(); // Clear timers, references, etc
    _delete_queue.push(uid);
}

template <class T, class Compare, class Allocator>
void WorldObjectContainer<T, Compare, Allocator>::delete_all()
{
    for (auto& it : _container)
    {
        delete (*it)->second;
    }
    _container.clear();
}

template <class T, class Compare, class Allocator>
void WorldObjectContainer<T, Compare, Allocator>::clear()
{
    _container.clear();
}

template <class T, class Compare, class Allocator>
size_t WorldObjectContainer<T, Compare, Allocator>::size()
{
    _container.size();
}

template <class T, class Compare, class Allocator>
void WorldObjectContainer<T, Compare, Allocator>::tick()
{
    if (!_delete_queue.empty())
    {
        for (auto& uid : _delete_queue)
        {
            delete _container[uid];
            _container.erase(uid);
        }
    }
}

#endif // __TORUS_GAME_WOC_H