#ifndef BRUSTSKETCHPERIOD_LEAKYBUCKET_H
#define BRUSTSKETCHPERIOD_LEAKYBUCKET_H


#include <cstdint>
#include <map>

class LeakyBucket {
    std::map<uint64_t,int> buckets;

public:
    LeakyBucket(int _windowSize, int _opNum, int _maxBucketSize);

    void insert(uint64_t _key, uint64_t t);
    void windowOperate();

public:
    int rejectNum_ = 0;

    int windowSize_;
    int limitNum_;

    int maxBucketSize_;
};

#endif //BRUSTSKETCHPERIOD_LEAKYBUCKET_H
