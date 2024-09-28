#include <set>
#include "LeakyBucket.h"

LeakyBucket::LeakyBucket(int _windowSize, int _limitNum, int _maxBucketSize):windowSize_(_windowSize), limitNum_(_limitNum), maxBucketSize_(_maxBucketSize) {}

void LeakyBucket::insert(uint64_t _key, uint64_t t) {
    if(buckets.find(_key) == buckets.end()){
        buckets[_key] = 1;
    }else{
        if(buckets[_key] < maxBucketSize_){
            buckets[_key]++;
        }else{
            rejectNum_++;
        }
    }

    if(t % windowSize_ == 0){
        windowOperate();
    }
}

void LeakyBucket::windowOperate() {
    for(auto &iter : buckets){
        iter.second -= limitNum_;
        if(iter.second < 0){
            iter.second = 0;
        }

    }
}

