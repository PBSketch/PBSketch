//
// Created by kalen on 2024/2/19.
//

#ifndef BRUSTSKETCHPERIOD_UPDOWNBURST_H
#define BRUSTSKETCHPERIOD_UPDOWNBURST_H

#include <cstdint>

template<typename ID_TYPE>
class UpDownBurst {
public:
    uint32_t start_window;
    uint32_t end_window;
    ID_TYPE id;


    UpDownBurst(){};
    UpDownBurst(uint32_t start, uint32_t end, ID_TYPE _id)
    {
        start_window = start;
        end_window = end;
        id = _id;

    }
};

#endif //BRUSTSKETCHPERIOD_UPDOWNBURST_H
