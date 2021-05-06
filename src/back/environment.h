//
// Created by 史记 on 2021/4/20.
//

#ifndef SYSYCOMPILER_ENVIRONMENT_H
#define SYSYCOMPILER_ENVIRONMENT_H

#include <map>

template<class K, class V>
class Environment {
private:
    Environment *outer;
    std::map<K, V *> *map;
public:
    Environment() : outer(nullptr), map(new std::map<K, V *>) {}

    explicit Environment(Environment *outer) : outer(outer), map(new std::map<K, V *>) {}

    [[nodiscard]] V *find(const K &key) {
        auto it = map->find(key);
        if (it != map->end())return it->second;
        else if (outer) return outer->find(key);
        else return nullptr;
    }

    bool insert(const K &key, V *val) {
        if (map->count(key))return false;
        map->insert({key, val});
        return true;
    }

    void newEnv() {
        outer = new Environment(outer);
        outer->map = map;
        map = new std::map<K, V *>;
    }

    void deleteEnv() {
        assert(outer);
        for (auto *v:*map)delete v;
        delete map;
        map = outer->map;
        outer = outer->outer;
    }
};


#endif //SYSYCOMPILER_ENVIRONMENT_H
