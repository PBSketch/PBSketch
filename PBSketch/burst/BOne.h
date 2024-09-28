#ifndef BRUSTSKETCHPERIOD_BURSTPASTSTAGEONE_H
#define BRUSTSKETCHPERIOD_BURSTPASTSTAGEONE_H

#include <cstdint>
#include <cstdlib>
#include "../../util/hash.h"

template<typename ID_TYPE, typename DATA_TYPE>
class BOne{

public:
    BOne(int len, uint32_t thres);
    ~BOne();
    void clearAll();
    void clearID(ID_TYPE id);
    DATA_TYPE insert(ID_TYPE id);


private:
    int array_len;
    uint32_t s1_thres;
    ID_TYPE *ids;
    DATA_TYPE *counters;
};

template<typename ID_TYPE, typename DATA_TYPE>
void BOne<ID_TYPE, DATA_TYPE>::clearAll() {
    memset(ids, 0, sizeof(ID_TYPE) * array_len);
    memset(counters, 0, sizeof(DATA_TYPE) * array_len);

}

template<typename ID_TYPE, typename DATA_TYPE>
void BOne<ID_TYPE, DATA_TYPE>::clearID(ID_TYPE id) {

    uint32_t index = hash(id,  20) % array_len;
    if (ids[index] == id) {
        ids[index] = 0;
        counters[index] = 0;
    }
}

template<typename ID_TYPE, typename DATA_TYPE>
BOne<ID_TYPE, DATA_TYPE>::~BOne() {
    delete [] ids;
    delete [] counters;
}

template<typename ID_TYPE, typename DATA_TYPE>
BOne<ID_TYPE, DATA_TYPE>::BOne(int len, uint32_t thres) : array_len(len),s1_thres(thres) {
        ids = new ID_TYPE[array_len];
        counters = new DATA_TYPE[array_len];
        memset(ids, 0, sizeof(ID_TYPE) * array_len);
        memset(counters, 0, sizeof(DATA_TYPE) * array_len);
}

template<typename ID_TYPE, typename DATA_TYPE>
DATA_TYPE BOne<ID_TYPE, DATA_TYPE>::insert(ID_TYPE id) {

    DATA_TYPE ret = 0;

    uint32_t index = hash(id, 20) % array_len;
    if (ids[index] == id) {
        if (++counters[index] > s1_thres)
            ret = counters[index];
    }
    else if (counters[index] == 0) {
        ++counters[index];
        ids[index] = id;
    }
    else {
        --counters[index];
    }
    return ret;
}

#endif //BRUSTSKETCHPERIOD_BURSTPASTSTAGEONE_H
