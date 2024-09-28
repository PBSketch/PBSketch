#ifndef PAIRUTIL_H
#define PAIRUTIL_H

#include <unordered_map>
#include <cstring>
#include <cstdint>
#include <chrono>
#include "periodichash.h"

typedef uint64_t DATA_TYPE;
typedef uint64_t TIME_TYPE;
typedef int32_t COUNT_TYPE;

    struct PTwoItemPair{
        TIME_TYPE time;
        DATA_TYPE item;

        PTwoItemPair(TIME_TYPE _time = 0, DATA_TYPE _item = 0):
                time(_time), item(_item){}
    };

bool operator == (const PTwoItemPair& a, const PTwoItemPair& b);


    typedef std::unordered_map<PTwoItemPair, COUNT_TYPE> PTwoItemPairHashMap;

namespace std{
    template<>
    struct hash<PTwoItemPair>{
        size_t operator()(const PTwoItemPair& item) const noexcept
        {
            return periodic::Hash::BOBHash32((uint8_t*)&item, sizeof(PTwoItemPair), 0);
        }
    };
}




#endif
