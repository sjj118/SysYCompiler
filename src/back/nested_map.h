//
// Created by 史记 on 2021/4/20.
//

#ifndef SYSYCOMPILER_NESTED_MAP_H
#define SYSYCOMPILER_NESTED_MAP_H

#include <map>

template<class K, class V>
class NestedMap {
private:
    NestedMap *outer;
    std::map<K, V> *map;
public:
    NestedMap() : outer(nullptr), map(new std::map<K, V>) {}

    explicit NestedMap(NestedMap *outer) : outer(outer), map(new std::map<K, V>) {}

    [[nodiscard]] V *find(const K &key) {
        auto it = map->find(key);
        if (it != map->end())return &it->second;
        else if (outer) return outer->find(key);
        else return nullptr;
    }

    bool insert(const K &key, const V &val) {
        if (map->count(key))return false;
        map->insert({key, val});
        return true;
    }

    void nest() {
        outer = new NestedMap(outer);
        outer->map = map;
        map = new std::map<K, V>;
    }

    void unnest() {
        assert(outer);
        delete map;
        map = outer->map;
        outer = outer->outer;
    }
};


#endif //SYSYCOMPILER_NESTED_MAP_H
