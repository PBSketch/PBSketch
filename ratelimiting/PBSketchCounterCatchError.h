#ifndef BRUSTSKETCHPERIOD_PBSKETCHCOUNTERCATCHERROR_H
#define BRUSTSKETCHPERIOD_PBSKETCHCOUNTERCATCHERROR_H

#include <map>
#include <cstdint>
#include <vector>
#include <queue>

#include "../PBSketch/PBSketch.h"

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
class PBSketchCounterCatchError {
public:
    std::map<uint64_t,int> counters;
    std::vector<uint64_t> bucketIds;
    std::vector<int> bucketValue;

    std::map<uint64_t, std::vector<uint64_t>[2]> checker;

    std::priority_queue<std::pair<uint64_t, uint64_t> > q;

    PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>*pbSketchWithWlPtr_;
public:
    PBSketchCounterCatchError(int _windowSize, int _limitNum, int _maxBucketSize, PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM> *_pbSketchWithWlPtr,int _maxBucketNum = 0);

    void insert(uint64_t _key, uint64_t t);
    void windowOperate(uint64_t _t);

public:
    int overflowNum_ = 0;
    int rejectNum_ = 0;

    int windowSize_;
    int limitNum_;
    int maxBucketNum_;
    int maxBucketSize_;

    int maxn = 0;
};

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM >
void PBSketchCounterCatchError<ID_TYPE, DATA_TYPE, CELL_NUM>::windowOperate(uint64_t _t) {
//    for(auto&iter:counters){
//        iter.second = 0;
//    }

    //In order to complete the experiment faster and reduce memory consumption, we chose to clear the counters to simulate clearing counter.
    // But in the real Rate Limiting scenes, we just set all counter to 0 like above.
    counters.clear();


    for(auto iter = checker.begin(); iter != checker.end();){
        if(iter->second[_t % 2].empty()){
            iter = checker.erase(iter);
        }else{
            iter++;
        }
    }

    for(auto& iter : checker){
        auto&past = iter.second[_t % 2 ^ 1];
        auto&now = iter.second[_t % 2];

        for(auto pastIter = past.begin(); pastIter != past.end(); pastIter++){
            int i;
            for(i = 0; i < now.size(); ++i){
                if(now[i] > *pastIter + windowSize_){
                    break;
                }
            }
            int pastResidue = std::distance(pastIter, past.end());
            if(i + pastResidue > limitNum_){
                overflowNum_++;
                break;
            }
        }
        past.clear();
    }

    int bucketNUm = bucketIds.size();
    std::set<uint64_t> st;

    for(int i = 0; i < bucketValue.size();){
        if(st.find(bucketIds[i]) == st.end()){
            st.insert(bucketIds[i]);
            bucketValue[i] -= limitNum_;
        }
        if(bucketValue[i] <= 0){
            bucketValue.erase(bucketValue.begin() + i);
            bucketIds.erase(bucketIds.begin() + i);
        }else{
            i++;
        }

    }


    maxn = std::max( bucketNUm , maxn);

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
            if(maxBucketNum_ == 0 || maxBucketNum_ > bucketIds.size()){
                bucketIds.push_back(head.second);
                bucketValue.push_back(0);
            }

        }

        q.pop();
    }

}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM >
void PBSketchCounterCatchError<ID_TYPE, DATA_TYPE, CELL_NUM>::insert(uint64_t _key, uint64_t _t) {

    pbSketchWithWlPtr_->insert(_key, _t);

    bool isHaveBucket = false;
    bool isInBucket = false;
    for(int i = 0; i < bucketIds.size(); ++i){
        if(bucketIds[i] == _key){
            isHaveBucket = true;
            if(bucketValue[i] < maxBucketSize_){
                isInBucket = true;
                bucketValue[i]++;
                break;
            }
        }
    }
    if(isHaveBucket && !isInBucket){
        rejectNum_++;
    }
    if(!isHaveBucket){
        if(counters.find(_key) == counters.end()){
            counters[_key] = 1;
        }else{
            if(counters[_key] < limitNum_){
                counters[_key]++;
            }else{
                rejectNum_++;
            }
        }
        int windowNum = _t / windowSize_;
        checker[_key][windowNum % 2].push_back(_t);
    }

    if(_t % windowSize_ == 0){
        windowOperate(_t / windowSize_);
    }
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
PBSketchCounterCatchError<ID_TYPE, DATA_TYPE, CELL_NUM>::PBSketchCounterCatchError(int _windowSize, int _limitNum, int _maxBucketSize,
                                                                                              PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM> *_pbSketchWithWlPtr,
                                                                                              int _maxBucketNum)
        :windowSize_(_windowSize), limitNum_(_limitNum), maxBucketSize_(_maxBucketSize),maxBucketNum_(_maxBucketNum),
         pbSketchWithWlPtr_(_pbSketchWithWlPtr){}

#endif //BRUSTSKETCHPERIOD_PBSKETCHCOUNTERCATCHERROR_H
