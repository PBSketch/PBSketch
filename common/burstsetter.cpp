//
// Created by kalen on 2024/7/13.
//

#include "burstsetter.h"

BurstSetter::BurstSetter(bool _countBased, uint64_t _len, int _winColdThres, int _lambda, uint32_t _burstThreshold) {
    if(_countBased){
        windowSize_ = _len;
    }else{
        windowTime_ = _len;
    }
    countBased_ = _countBased;
    winColdThres_ = _winColdThres;
    lambda_ = _lambda;
    burstThreshold_ = _burstThreshold;
}
