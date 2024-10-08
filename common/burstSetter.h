#ifndef BRUSTSKETCHPERIOD_BURSTSETTER_H
#define BRUSTSKETCHPERIOD_BURSTSETTER_H

#include<stdint.h>

class BurstSetter{
public:
    bool countBased_;
    uint32_t windowSize_;
    uint64_t windowTime_;
    int winColdThres_;
    int lambda_;
    uint32_t burstThreshold_;

    BurstSetter(bool _countBased, uint64_t _len, int _winColdThres, int _lambda, uint32_t _burstThreshold);
};

#endif //BRUSTSKETCHPERIOD_BURSTPARTSETTER_H
