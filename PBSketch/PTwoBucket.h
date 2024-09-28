//
// Created by kalen on 2024/2/20.
//

#ifndef BRUSTSKETCHPERIOD_PTWOBUCKET_H
#define BRUSTSKETCHPERIOD_PTWOBUCKET_H

#include "pairutil.h"

struct PTwoCell{
    DATA_TYPE item = 0;
    uint32_t interval = 0;
    COUNT_TYPE count = 0;
};

template <uint32_t CELL_NUM>
struct PTwoBucket{

    PTwoCell cells[CELL_NUM];
    int fail = 0;

};


#endif //BRUSTSKETCHPERIOD_PTWOBUCKET_H
