#include "CounterCatchError.h"

CounterCatchError::CounterCatchError(int _windowSize, int _limitNum) : windowSize_(_windowSize), limitNum_(_limitNum){}

void CounterCatchError::insert(uint64_t _key, uint64_t t) {
    if(counters.find(_key) == counters.end()){
        counters[_key] = 1;
    }else{
        if(counters[_key] < limitNum_){
            counters[_key]++;
        }else{
            rejectNum_++;
        }
    }

    int windowNum = t / windowSize_;
    checker[_key][windowNum % 2].push_back(t);

    if(t % windowSize_ == 0){
        windowOperate(windowNum);
    }
}

void CounterCatchError::windowOperate(uint64_t windowNum) {
//    for(auto&iter:counters){
//        iter.second = 0;
//    }

    //In order to complete the benchmark faster and reduce memory consumption, we chose to clear the counters to simulate clearing counter.
    // But in the real Rate Limiting scenes, we just set all counter to 0 like above.
    counters.clear();
    for(auto iter = checker.begin(); iter != checker.end();){
        if(iter->second[windowNum % 2].empty()){
            iter = checker.erase(iter);
        }else{
            iter++;
        }
    }

    for(auto& iter : checker){
        auto&past = iter.second[windowNum % 2 ^ 1];
        auto&now = iter.second[windowNum % 2];

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
}