//
// Created by kalen on 2024/2/20.
//

#ifndef BRUSTSKETCHPERIOD_GSUBUCKET_H
#define BRUSTSKETCHPERIOD_GSUBUCKET_H

#include "periodicutil.h"

struct Cell{
    DATA_TYPE item = 0;
    uint32_t interval = 0;
    COUNT_TYPE count = 0;
};

template <uint32_t CELL_NUM>
struct GSUBucket{

    Cell cells[CELL_NUM];
    int fail = 0;

};


#endif //BRUSTSKETCHPERIOD_PTWOBUCKET_H
