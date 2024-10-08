#ifndef BRUSTSKETCHPERIOD_PBSKETCH_H
#define BRUSTSKETCHPERIOD_PBSKETCH_H

#include "periodicPart.h"
#include "burst/BOne.h"
#include "burst/BTwo.h"

#include "../common/peridoc.hpp"
#include "../common/burstSetter.h"
#include "burst/burstpartsetter.h"
#include "periodicpartsetter.h"



template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
class PBSketch {
public:
    void insert(ID_TYPE _id, uint64_t _timestamp);
    void windowTransition(const std::multiset<uint64_t>&_searcher);
    PTwoItemPairHashMap Report(uint64_t _hit);
    std::vector<PBflow> ReportTop();
    uint32_t setWindowsSize(uint32_t _size);
    uint64_t setWindowsTime(uint32_t _time);
    void setCountBaseWindows(uint32_t _size, int _winColdThres, int _lambda, uint32_t _burstThreshold);
    void setTimeBaseWindows(uint32_t _time, int _winColdThres, int _lambda, uint32_t _burstThreshold);
    void setBurst(BurstSetter _burstSetter);
    void setBurstPart(int _kickMaxNum, int _b2BucketSize, int _burstMem, double _burstStageRatio, uint32_t _burstS1Thres);
    void setBurstPart(BurstPartSetter _burstPartSetter);
    void setPeriodicPart(int _periodicMemory, double _periodicPartRatio, double _delta, int _topK);
    void setPeriodicPart(PeriodicPartSetter _periodicPartSetter);
    PBSketch(BurstSetter _burstSetter, BurstPartSetter _burstPartSetter, PeriodicPartSetter _periodicPartSetter);
    ~PBSketch();

    std::vector<UpDownBurst<ID_TYPE>> burstResult;
public:
    bool countBased_;
    uint32_t window_size;
    uint64_t window_time;
    int win_cold_thres;
    int lambda;
    uint32_t burstThreshold_;

    int kickMaxNum_;
    int b2BucketSize_;
    int burstMem_;
    double burstPartRatio_;
    uint32_t burstS1Thres_;


    int periodicMemory_;
    double periodicPartRatio_;
    double delta_;
    int topK_;

protected:
    void initBrust(int mem, double stage_ratio, uint32_t s1_thres);

public:
    uint64_t start_time{};
    uint64_t last_timestamp = 0;
    uint32_t win_cnt = 0;
    BOne<ID_TYPE, DATA_TYPE> *b1 = nullptr;
    BTwo<ID_TYPE, DATA_TYPE> *b2 = nullptr;

    PeriodicPart<CELL_NUM> periodicPart_;

public:
    std::vector<UpDownBurst<ID_TYPE>> burstsLastWindow_;

};

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::PBSketch(BurstSetter _burstSetter,
                                                                                 BurstPartSetter _burstPartSetter,
                                                                                 PeriodicPartSetter _periodicPartSetter):
                                                                                 periodicPart_(_periodicPartSetter){
    setBurst(_burstSetter);
    setBurstPart(_burstPartSetter);
    setPeriodicPart(_periodicPartSetter);
    if (!countBased_) {
        last_timestamp = start_time;
    }
    else {
        last_timestamp = 0;
    }
    initBrust( burstMem_, burstPartRatio_, burstS1Thres_);

}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
void PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::setPeriodicPart(PeriodicPartSetter _periodicPartSetter) {
    setPeriodicPart(_periodicPartSetter.periodicMemory_, _periodicPartSetter.periodicPartRatio_, _periodicPartSetter.delta_, _periodicPartSetter.topK_);
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
void
PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::setPeriodicPart(int _periodicMemory, double _periodicPartRatio,
                                                                             double _delta, int _topK) {
    periodicMemory_ = _periodicMemory;
    periodicPartRatio_ = _periodicPartRatio;
    delta_ = _delta;
    topK_ = _topK;
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
void PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::setBurstPart(BurstPartSetter _burstPartSetter) {
    setBurstPart(_burstPartSetter.burstHashNum_, _burstPartSetter.stage2BucketSize_, _burstPartSetter.burstMem_, _burstPartSetter.burstStageRatio_, _burstPartSetter.burstS1Thres_);
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
void PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::setBurstPart(int _kickMaxNum, int _b2BucketSize,
                                                                               int _burstMem, double _burstStageRatio,
                                                                               uint32_t _burstS1Thres) {
    kickMaxNum_ = _kickMaxNum;
    b2BucketSize_ = _b2BucketSize;
    burstMem_ = _burstMem;
    burstPartRatio_ = _burstStageRatio;
    burstS1Thres_ = _burstS1Thres;

}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
void PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::setBurst(BurstSetter _burstSetter) {
    if(_burstSetter.countBased_){
        setCountBaseWindows(_burstSetter.windowSize_, _burstSetter.winColdThres_, _burstSetter.lambda_,_burstSetter.burstThreshold_);
    }else{
        setTimeBaseWindows(_burstSetter.windowTime_, _burstSetter.winColdThres_, _burstSetter.lambda_,_burstSetter.burstThreshold_);
    }
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
void PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::setTimeBaseWindows(uint32_t _time, int _winColdThres,
                                                                                     int _lambda,
                                                                                     uint32_t _burstThreshold) {
    setWindowsTime(_time);
    win_cold_thres = _winColdThres;
    lambda = _lambda;
    burstThreshold_ = _burstThreshold;
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
void
PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::setCountBaseWindows(uint32_t _size, int _winColdThres,
                                                                                 int _lambda,
                                                                                 uint32_t _burstThreshold) {
    setWindowsSize(_size);
    win_cold_thres = _winColdThres;
    lambda = _lambda;
    burstThreshold_ = _burstThreshold;
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
uint64_t PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::setWindowsTime(uint32_t _time) {
    countBased_ = false;
    window_time = _time;
    return _time;
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
uint32_t PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::setWindowsSize(uint32_t _size) {
    countBased_ = true;
    window_size = _size;
    return _size;
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
void PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::windowTransition(
        const std::multiset<uint64_t> &_searcher) {
    b1->clearAll();
    b2->window_transition(win_cnt++, _searcher);
//    burstResult.insert(burstResult.end(), b2->results.begin(), b2->results.end());
    burstsLastWindow_.clear();
    burstsLastWindow_.insert(burstsLastWindow_.end(), b2->results.begin(), b2->results.end());

    for(int i = 0; i < b2->results.size(); i++){
        auto iter = b2->results[i];
        PTwoItemPair itemPair;
        itemPair.item = iter.id;
        itemPair.time = iter.start_window;
        periodicPart_.Insert(itemPair);
    }
    b2->results.clear();

}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::~PBSketch() {
    burstResult.clear();

}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
std::vector<PBflow> PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::ReportTop() {
    return periodicPart_.ReportTop();
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
void PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::insert(ID_TYPE _id, uint64_t _timestamp) {
    DATA_TYPE count;
    if (countBased_) {
        while (last_timestamp + window_size < _timestamp) {
            // transition to next window
            last_timestamp += window_size;

            windowTransition(periodicPart_.heap.searcher_);
        }
    }
    else {
        while (last_timestamp + window_time < _timestamp) {
            // transition to next window
            last_timestamp += window_time;

            windowTransition(periodicPart_.heap.searcher_);

        }
    }
    if (b2->lookup(_id)) {
        return;
    }
    count = b1->insert(_id);

    if (count != 0 && b2->insert(_id, _timestamp, count, periodicPart_.heap.searcher_)) {

        b1->clearID(_id);
    }
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
void PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::initBrust(int mem, double stage_ratio, uint32_t s1_thres) {
    int b1_mem = mem * 1024 * stage_ratio / (1 + stage_ratio);
    int b2_mem = mem * 1024 / (1 + stage_ratio);
    int b1_len = b1_mem / (sizeof(ID_TYPE) + sizeof(DATA_TYPE));
    int b2_size = b2BucketSize_;
    int b2_num = b2_mem / b2_size / (sizeof(int32_t) + sizeof (ID_TYPE) + 2 * sizeof(DATA_TYPE));
    b1 = new BOne<ID_TYPE,DATA_TYPE>(b1_len, s1_thres);
    b2 = new BTwo<ID_TYPE,DATA_TYPE>(b2_num, b2_size, s1_thres, burstThreshold_, kickMaxNum_);

    b2->lambda = lambda;
    b2->win_cold_thres = win_cold_thres;


}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM>
PTwoItemPairHashMap PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>::Report(uint64_t _hit) {
    return periodicPart_.Report(_hit);
}



#endif //BRUSTSKETCHPERIOD_PBSKETCHWITHWL_H
