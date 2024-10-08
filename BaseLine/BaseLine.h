#ifndef BRUSTSKETCHPERIOD_BURSTSKETCHADDPERIODICSKETCH_H
#define BRUSTSKETCHPERIOD_BURSTSKETCHADDPERIODICSKETCH_H

#include "PeriodicSketch.h"
#include "burst/BurstSketchStageOne.h"
#include "burst/BurstSketchStageTwo.h"

#include "../common/peridoc.hpp"
#include "../common/burstSetter.h"
#include "burst/burstpartsetter.h"
#include "periodicpartsetter.h"


template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
class BaseLine {
public:
    void insert(ID_TYPE _id, uint64_t _timestamp);
    void windowTransition();
    PerodicHashMap Report(uint64_t _hit);
    std::vector<PBflow> ReportTop();
    uint32_t setWindowsSize(uint32_t _size);
    uint64_t setWindowsTime(uint32_t _time);
    void setCountBaseWindows(uint32_t _size, int _winColdThres, int _lambda, uint32_t _burstThreshold);
    void setTimeBaseWindows(uint32_t _time, int _winColdThres, int _lambda, uint32_t _burstThreshold);
    void setBurst(BurstSetter _burstSetter);
    void setBurstPart(int _burstHashNum, int _stage2BucketSize, int _burstMem, double _burstStageRatio, uint32_t _burstS1Thres);
    void setBurstPart(BurstPartSetter _burstPartSetter);
    void setPeriodicPart(int _periodicMemory, double _CMGSUratio, double _delta, int _topK);
    void setPeriodicPart(PeriodicPartSetter _periodicPartSetter);
    BaseLine(BurstSetter _burstSetter, BurstPartSetter _burstPartSetter, PeriodicPartSetter _periodicPartSetter);
    ~BaseLine();

    std::vector<UpDownBurst<ID_TYPE>> burstResult;
public:
    bool countBased_;
    uint32_t window_size;
    uint64_t window_time;
    int win_cold_thres;
    int lambda;
    uint32_t burstThreshold_;

    int burstHashNum_;
    int stage2BucketSize_;
    int burstMem_;
    double burstStageRatio_;
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
    BurstSketchStageOne<ID_TYPE, DATA_TYPE> *stage1 = nullptr;
    BurstSketchStageTwo<ID_TYPE, DATA_TYPE> *stage2 = nullptr;

    PeriodicSketch<HASH_NUM,CELL_NUM> periodicPart_;

public:
    std::vector<UpDownBurst<ID_TYPE>> burstsLastWindow_;

};

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
std::vector<PBflow> BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::ReportTop() {
    return periodicPart_.ReportTop();
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::BaseLine(BurstSetter _burstSetter,
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
    initBrust( burstMem_, burstStageRatio_, burstS1Thres_);

}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
void BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::setPeriodicPart(PeriodicPartSetter _periodicPartSetter) {
    setPeriodicPart(_periodicPartSetter.periodicMemory_, _periodicPartSetter.periodicPartRatio_, _periodicPartSetter.delta_, _periodicPartSetter.topK_);
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
void
BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::setPeriodicPart(int _periodicMemory, double _periodicPartRatio,
                                                                             double _delta, int _topK) {
    periodicMemory_ = _periodicMemory;
    periodicPartRatio_ = _periodicPartRatio;
    delta_ = _delta;
    topK_ = _topK;
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
void BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::setBurstPart(BurstPartSetter _burstPartSetter) {
    setBurstPart(_burstPartSetter.burstHashNum_, _burstPartSetter.stage2BucketSize_, _burstPartSetter.burstMem_, _burstPartSetter.burstStageRatio_, _burstPartSetter.burstS1Thres_);
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
void BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::setBurstPart(int _burstHashNum, int _stage2BucketSize,
                                                                               int _burstMem, double _burstStageRatio,
                                                                               uint32_t _burstS1Thres) {
    burstHashNum_ = _burstHashNum;
    stage2BucketSize_ = _stage2BucketSize;
    burstMem_ = _burstMem;
    burstStageRatio_ = _burstStageRatio;
    burstS1Thres_ = _burstS1Thres;

}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
void BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::setBurst(BurstSetter _burstSetter) {
    if(_burstSetter.countBased_){
        setCountBaseWindows(_burstSetter.windowSize_, _burstSetter.winColdThres_, _burstSetter.lambda_,_burstSetter.burstThreshold_);
    }else{
        setTimeBaseWindows(_burstSetter.windowTime_, _burstSetter.winColdThres_, _burstSetter.lambda_,_burstSetter.burstThreshold_);
    }
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
void BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::setTimeBaseWindows(uint32_t _time, int _winColdThres,
                                                                                     int _lambda,
                                                                                     uint32_t _burstThreshold) {
    setWindowsTime(_time);
    win_cold_thres = _winColdThres;
    lambda = _lambda;
    burstThreshold_ = _burstThreshold;
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
void
BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::setCountBaseWindows(uint32_t _size, int _winColdThres,
                                                                                 int _lambda,
                                                                                 uint32_t _burstThreshold) {
    setWindowsSize(_size);
    win_cold_thres = _winColdThres;
    lambda = _lambda;
    burstThreshold_ = _burstThreshold;
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
uint64_t BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::setWindowsTime(uint32_t _time) {
    countBased_ = false;
    window_time = _time;
    return _time;
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
uint32_t BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::setWindowsSize(uint32_t _size) {
    countBased_ = true;
    window_size = _size;
    return _size;
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
void BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::windowTransition() {
    stage1->clearAll();
    stage2->window_transition(win_cnt++);
    burstResult.insert(burstResult.end(), stage2->results.begin(), stage2->results.end());

    for(int i = 0; i < stage2->results.size(); i++){
        auto iter = stage2->results[i];
        PeridocItemPair itemPair;
        itemPair.item = iter.id;
        itemPair.time = iter.start_window;
        periodicPart_.Insert(itemPair);
    }
    stage2->results.clear();

}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::~BaseLine() {
    burstResult.clear();

}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
void BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::insert(ID_TYPE _id, uint64_t _timestamp) {
    DATA_TYPE count;
    if (countBased_) {
        while (last_timestamp + window_size < _timestamp) {
            // transition to next window
            last_timestamp += window_size;

            windowTransition();
        }
    }
    else {
        while (last_timestamp + window_time < _timestamp) {
            // transition to next window
            last_timestamp += window_time;

            windowTransition();

        }
    }
    if (stage2->lookup(_id)) {
        return;
    }
    count = stage1->insert(_id);

    if (count != 0 && stage2->insert(_id, _timestamp, count)) {

        stage1->clearID(_id);
    }
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
void BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::initBrust(int mem, double stage_ratio, uint32_t s1_thres) {
    int stage1_mem = mem * 1024 * stage_ratio / (1 + stage_ratio);
    int stage2_mem = mem * 1024 / (1 + stage_ratio);
    int stage1_num = burstHashNum_;
    int stage1_len = stage1_mem / stage1_num / (sizeof(ID_TYPE) + sizeof(DATA_TYPE));
    int stage2_size = stage2BucketSize_;
    int stage2_num = stage2_mem / stage2_size / (sizeof(int32_t) + sizeof (ID_TYPE) + 2 * sizeof(DATA_TYPE));
    stage1 = new BurstSketchStageOne<ID_TYPE,
            DATA_TYPE>(stage1_num, stage1_len, s1_thres);
    stage2 = new BurstSketchStageTwo<ID_TYPE,
            DATA_TYPE>(stage2_num, stage2_size, s1_thres, burstThreshold_);

    stage2->lambda = lambda;
    stage2->win_cold_thres = win_cold_thres;


}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
PerodicHashMap BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>::Report(uint64_t _hit) {
    return periodicPart_.Report(_hit);
}



#endif //BRUSTSKETCHPERIOD_PBSKETCHWITHWL_H
