#ifndef BRUSTSKETCHPERIOD_PERIODICPARTSETTER_H
#define BRUSTSKETCHPERIOD_PERIODICPARTSETTER_H

struct PeriodicPartSetter{
    int periodicMemory_;
    double periodicPartRatio_;
    double delta_;
    int topK_;

    PeriodicPartSetter(int _periodicMemory, double _periodicPartRatio, double _delta, int _topK){
        periodicMemory_ = _periodicMemory;
        periodicPartRatio_ = _periodicPartRatio;
        delta_ = _delta;
        topK_ = _topK;
    }

};

#endif //BRUSTSKETCHPERIOD_PERIODICPARTSETTER_H
