#ifndef BRUSTSKETCHPERIOD_PBSKETCHLEAKYBUCKET_H
#define BRUSTSKETCHPERIOD_PBSKETCHLEAKYBUCKET_H

#include <map>
#include <cstdint>
#include <vector>
#include <queue>

#include "../PBSketch/PBSketch.h"


template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
class PBSketchLeakyBucket {
public:
    std::map<uint64_t,int> buckets;
    std::vector<uint64_t> bucketIds;
    std::vector<int> bucketValue;
    std::priority_queue<std::pair<uint64_t, uint64_t> > q;

    PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>*pbSketchWithWlPtr_;
public:
    PBSketchLeakyBucket(int _windowSize, int _limitNum, int _maxBucketNum, int _dynamicBucketCapacity,
                        PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM> *_pbSketchWithWlPtr, int _dynamicBucketMaxNum = 0);

    void insert(uint64_t _key, uint64_t t);
    void windowOperate(uint64_t _t);

public:
    int rejectNum_ = 0;

    int windowSize_;
    int limitNum_;
    int maxBucketSize_;
    int dynamicBucketCapacity_;
    int dynamicBucketMaxNum_;

    int maxn = 0;
};

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM >
void PBSketchLeakyBucket<ID_TYPE, DATA_TYPE, CELL_NUM>::windowOperate(uint64_t _t) {

    int bucketNUm = bucketIds.size();
    std::map<uint64_t, int> mp;

    for(auto &iter : buckets){

        if(iter.second < limitNum_){
            mp[iter.second] = limitNum_ - iter.second;
            iter.second = 0;
        }else{
            iter.second -= limitNum_;
            mp[iter.second] = 0;
        }
    }
    for(int i = 0; i < bucketValue.size();){
        if(mp.find(bucketIds[i]) == mp.end()){
            if(bucketValue[i] < limitNum_){
                mp[bucketIds[i]] = limitNum_ - bucketValue[i];
                bucketValue[i] = 0;
            }else{
                bucketValue[i] -= limitNum_;
                mp[bucketIds[i]] = 0;
            }
        }else{
            if(mp[bucketIds[i]] != 0){
                if(bucketValue[i] < mp[bucketIds[i]]){
                    mp[bucketIds[i]] = mp[bucketIds[i]] - bucketValue[i];
                    bucketValue[i] = 0;
                }else{
                    bucketValue[i] -= mp[bucketIds[i]];
                    mp[bucketIds[i]] = 0;
                }
            }
        }
        if(bucketValue[i] <= 0){
            bucketValue.erase(bucketValue.begin() + i);
            bucketIds.erase(bucketIds.begin() + i);
        }else{
            i++;
        }

    }



    maxn = std::max(bucketNUm,maxn);

    auto&bursts = pbSketchWithWlPtr_->burstsLastWindow_;
    auto periodicResult = pbSketchWithWlPtr_->ReportTop();
    for(int i = 0; i < bursts.size(); ++i){

        for(auto& iter : periodicResult){

            uint64_t itemKey = iter.peridocItem_.itemKey_;
            uint64_t peridoc = iter.peridocItem_.peridoc_;


            if(bursts[i].id == itemKey){
                q.push(std::make_pair(bursts[i].start_window + peridoc - 1, bursts[i].id) );
            }
        }
    }

    while(!q.empty() && q.top().first < _t){
        auto head = q.top();
        bool flag = true;
        for(unsigned long bucketId : bucketIds){
            if(bucketId == head.second){
                flag = false;
                break;
            }
        }
        if(flag) {
            if(dynamicBucketMaxNum_ == 0 || dynamicBucketMaxNum_ > bucketIds.size() ){
                bucketIds.push_back(head.second);
                bucketValue.push_back(0);
            }

        }
        q.pop();
    }

}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
void PBSketchLeakyBucket<ID_TYPE, DATA_TYPE, CELL_NUM>::insert(uint64_t _key, uint64_t _t) {

    pbSketchWithWlPtr_->insert(_key, _t);

    if(buckets.find(_key) == buckets.end()){
        buckets[_key] = 1;
    }else{
        if(buckets[_key] < maxBucketSize_){
            buckets[_key]++;
        }else{
            bool flag = true;
            for(int i = 0; i < bucketIds.size(); ++i){
                if(bucketIds[i] == _key && bucketValue[i] < dynamicBucketCapacity_){
                    bucketValue[i]++;
                    flag = false;
                    break;
                }
            }
            if(flag){
                rejectNum_++;
            }
        }
    }

    if(_t % windowSize_ == 0){
        windowOperate(_t / windowSize_);
    }
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
PBSketchLeakyBucket<ID_TYPE, DATA_TYPE, CELL_NUM>::PBSketchLeakyBucket(int _windowSize, int _limitNum, int _maxBucketSize, int _dynamicBucketCapacity,
                                                                                  PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM> *_pbSketchWithWlPtr, int _dynamicBucketMaxNum)
        :windowSize_(_windowSize), limitNum_(_limitNum), maxBucketSize_(_maxBucketSize), dynamicBucketCapacity_(_dynamicBucketCapacity),
         dynamicBucketMaxNum_(_dynamicBucketMaxNum),pbSketchWithWlPtr_(_pbSketchWithWlPtr){}

#endif //BRUSTSKETCHPERIOD_PBSKETCHLEAKYBUCKET_H
