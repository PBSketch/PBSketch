#ifndef PERIODICUTIL_H
#define PERIODICUTIL_H

#include <unordered_map>
#include <cstring>
#include <cstdint>
#include <chrono>
#include "periodichash.h"


typedef uint64_t DATA_TYPE;
typedef uint64_t TIME_TYPE;
typedef int32_t COUNT_TYPE;

    struct PeridocItemPair{
        TIME_TYPE time;
        DATA_TYPE item;
        uint32_t amplitude;

        PeridocItemPair(TIME_TYPE _time = 0, DATA_TYPE _item = 0, uint32_t _amplitude = 0):
                time(_time), item(_item), amplitude(_amplitude){}
    };

bool operator == (const PeridocItemPair& a, const PeridocItemPair& b);


    typedef std::unordered_map<PeridocItemPair, COUNT_TYPE> PerodicHashMap;

namespace std{
    template<>
    struct hash<PeridocItemPair>{
        size_t operator()(const PeridocItemPair& item) const noexcept
        {
            return periodic::Hash::BOBHash32((uint8_t*)&item, sizeof(PeridocItemPair), 0);
        }
    };
}




#endif
