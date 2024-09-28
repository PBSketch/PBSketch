//
// Created by kalen on 2024/7/12.
//

#ifndef BRUSTSKETCHPERIOD_BURSTPARTSETTER_H
#define BRUSTSKETCHPERIOD_BURSTPARTSETTER_H

struct BurstPartSetter{
    int burstHashNum_;
    int stage2BucketSize_;
    int burstMem_;
    double burstStageRatio_;
    uint32_t burstS1Thres_;

    BurstPartSetter(int _burstHashNum, int _stage2BucketSize, int _burstMem, double _burstStageRatio, uint32_t _burstS1Thres){
        burstHashNum_ = _burstHashNum;
        stage2BucketSize_ = _stage2BucketSize;
        burstMem_ = _burstMem;
        burstStageRatio_ = _burstStageRatio;
        burstS1Thres_ = _burstS1Thres;
    }
};

#endif //BRUSTSKETCHPERIOD_BURSTPARTSETTER_H
