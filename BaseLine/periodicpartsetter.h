//
// Created by kalen on 2024/7/12.
//

#ifndef BRUSTSKETCHPERIOD_PERIODICPARTSETTER_H
#define BRUSTSKETCHPERIOD_PERIODICPARTSETTER_H

struct PeriodicPartSetter{
    int periodicMemory_;
    double CMGSUratio_;
    double delta_;
    int topK_;

    PeriodicPartSetter(int _periodicMemory, double _CMGSUratio, double _delta, int _topK){
        periodicMemory_ = _periodicMemory;
        CMGSUratio_ = _CMGSUratio;
        delta_ = _delta;
        topK_ = _topK;
    }

};

#endif //BRUSTSKETCHPERIOD_PERIODICPARTSETTER_H
