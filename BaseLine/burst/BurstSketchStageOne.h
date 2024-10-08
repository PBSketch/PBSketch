#ifndef BRUSTSKETCHPERIOD_BURSTSKETCHSTAGEONE_H
#define BRUSTSKETCHPERIOD_BURSTSKETCHSTAGEONE_H

#include <cstdint>
#include <cstdlib>
#include "../../common/hash.h"

template<typename ID_TYPE, typename DATA_TYPE>
class BurstSketchStageOne{

public:
    BurstSketchStageOne(int num, int len, uint32_t thres);
    ~BurstSketchStageOne();
    void clearAll();
    void clearID(ID_TYPE id);
    DATA_TYPE insert(ID_TYPE id);


private:
    int array_num, array_len;
    uint32_t s1_thres;
    ID_TYPE **ids;
    DATA_TYPE **counters;
};

template<typename ID_TYPE, typename DATA_TYPE>
void BurstSketchStageOne<ID_TYPE, DATA_TYPE>::clearAll() {
    for (int i = 0; i < array_num; ++i) {
        memset(ids[i], 0, sizeof(ID_TYPE) * array_len);
        memset(counters[i], 0, sizeof(DATA_TYPE) * array_len);
    }

}

template<typename ID_TYPE, typename DATA_TYPE>
void BurstSketchStageOne<ID_TYPE, DATA_TYPE>::clearID(ID_TYPE id) {
    for (int i = 0; i < array_num; ++i) {
        uint32_t index = hash(id, i + 20) % array_len;
        if (ids[i][index] == id) {
            ids[i][index] = 0;
            counters[i][index] = 0;
        }
    }
}

template<typename ID_TYPE, typename DATA_TYPE>
BurstSketchStageOne<ID_TYPE, DATA_TYPE>::~BurstSketchStageOne() {
    for (int i = 0; i < array_num; ++i) {
        delete [] ids[i];
        delete [] counters[i];
    }
    delete [] ids;
    delete [] counters;
}

template<typename ID_TYPE, typename DATA_TYPE>
BurstSketchStageOne<ID_TYPE, DATA_TYPE>::BurstSketchStageOne(int num, int len, uint32_t thres) : array_num(num), array_len(len),
                                                                                                   s1_thres(thres) {
    ids = new ID_TYPE*[array_num];
    counters = new DATA_TYPE*[array_num];
    for (int i = 0; i < array_num; ++i) {
        ids[i] = new ID_TYPE[array_len];
        counters[i] = new DATA_TYPE[array_len];
        memset(ids[i], 0, sizeof(ID_TYPE) * array_len);
        memset(counters[i], 0, sizeof(DATA_TYPE) * array_len);
    }
}

template<typename ID_TYPE, typename DATA_TYPE>
DATA_TYPE BurstSketchStageOne<ID_TYPE, DATA_TYPE>::insert(ID_TYPE id) {
#ifdef DEGBUG
    insertStageOne++;
#endif

    DATA_TYPE ret = 0;
    for (int i = 0; i < array_num; ++i) {
        uint32_t index = hash(id, i + 20) % array_len;
        if (ids[i][index] == id) {
            if (++counters[i][index] > s1_thres)
                ret = counters[i][index];
        }
        else if (counters[i][index] == 0) {
            ++counters[i][index];
            ids[i][index] = id;
        }
        else {

                --counters[i][index];

        }
    }
    return ret;
}

#endif //BRUSTSKETCHPERIOD_BURSTPASTSTAGEONE_H
