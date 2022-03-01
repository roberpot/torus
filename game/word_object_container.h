#ifndef __TORUS_GAME_WOC_H
#define __TORUS_GAME_WOC_H

#include <library/stack.h>
#include <map>
#include <game/uid.h>

class Char;

#define WOCDef WorldObjectContainer<T, Compare, Allocator>
#define WOCSimple <WorldObjectContainer<T>

#define WOCTemp <class T, class Compare = std::less<Uid>, class Allocator = std::allocator<std::pair<const Uid, T>>>
#define WOCMethod template <class T, class Compare, class Allocator>



template WOCTemp
class WorldObjectContainer
{
    std::map<Uid,T, Compare, Allocator> _container;
    ttl::dynamicstack<T> _delete_queue;
public:
    void add(Uid uid, T object);
    bool has(T object);
    bool has_uid(Uid uid);
    T get(T object);
    void remove(T object);
    void delete_now(T object);
    void delete_later(T object);
    void delete_all();
    void clear();
    size_t size();
};


WOCMethod
void WOCDef::add(Uid uid, T object)
{
    if (!_container.empty())
    {
        if (_container.find(uid) != _container.end())
        {
            //err?
            return;
        }
    }
    
    _container[uid] = object;
}

WOCMethod
bool WOCDef::has(T object)
{
    return _container.find(object) != _container.end();
}

WOCMethod
bool WOCDef::has_uid(Uid uid)
{
    return _container.at(uid) != nullptr;
}

WOCMethod
T WOCDef::get(T object)
{
    T ret = nullptr;
    auto it = _container.find(t);
    if (it != _container.end())
    {
        ret = (*it)->second;
    }
    return ret;
}

WOCMethod
void WOCDef::remove(T object)
{
    auto it = _container.find(object);
    if (it != _container.end())
    {
        _container.erase(it);
    }
}

WOCMethod
void WOCDef::delete_now(T object)
{
    remove(object);
    delete object;
}

WOCMethod
void WOCDef::delete_later(T object)
{
    remove(object);
    //TODO object->delete_later(); // Clear timers, references, etc
    _delete_queue.push(object);
}

WOCMethod
void WOCDef::delete_all()
{
    for (auto& it : _container)
    {
        delete (*it)->second;
    }
}

WOCMethod
void WOCDef::clear()
{
    _container.clear();
}

WOCMethod
size_t WOCDef::size()
{
    _container.size();
}

#endif // __TORUS_GAME_WOC_H