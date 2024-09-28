#ifndef _CORRECTDETECTOR_H_
#define _CORRECTDETECTOR_H_

#include "../common/Burst.h"

#include <map>
#include <vector>
#include <assert.h>
#include "iostream"

#include "../common/burstSetter.h"


template<typename ID_TYPE>
struct BurstCorrectDetectItem {
    uint32_t counters[2];
    int32_t window;
    ID_TYPE id;

    BurstCorrectDetectItem() {}
    BurstCorrectDetectItem(ID_TYPE _id) : id(_id) {
        counters[0] = counters[1] = 0;
        window = -1;
    }
    BurstCorrectDetectItem(const BurstCorrectDetectItem &item) {
        counters[0] = item.counters[0];
        counters[1] = item.counters[1];
        window = item.window;
        id = item.id;
    }
};

template<typename ID_TYPE>
class BurstCorrectDetector {
public:
    bool countBased_;
    uint32_t windowSize_;
    uint64_t windowTime_;
    int winColdThres_;
    int lambda_;
    uint32_t burstThreshold_;

public:


    BurstCorrectDetector(BurstSetter _burstSetter): last_timestamp(0), win_cnt(0), flag(0){
        if(_burstSetter.countBased_){
            setCountBaseWindows(_burstSetter.windowSize_, _burstSetter.winColdThres_, _burstSetter.lambda_,_burstSetter.burstThreshold_);
        }else{
            setTimeBaseWindows(_burstSetter.windowTime_, _burstSetter.winColdThres_, _burstSetter.lambda_,_burstSetter.burstThreshold_);
        }

    }

	~BurstCorrectDetector() {}

    void setTimeBaseWindows(uint32_t _time, int _winColdThres, int _lambda, uint32_t _burstThreshold) {
        setWindowsTime(_time);
        winColdThres_ = _winColdThres;
        lambda_ = _lambda;
        burstThreshold_ = _burstThreshold;
    }

    void setCountBaseWindows(uint32_t _size, int _winColdThres, int _lambda, uint32_t _burstThreshold) {
        setWindowsSize(_size);
        winColdThres_ = _winColdThres;
        lambda_ = _lambda;
        burstThreshold_ = _burstThreshold;
    }

    uint64_t setWindowsTime(uint32_t _time) {
        countBased_ = false;
        windowTime_ = _time;
        return _time;
    }

    uint32_t setWindowsSize(uint32_t _size) {
        countBased_ = true;
        windowSize_ = _size;
        return _size;
    }

    void transition(uint64_t window) { // is the logic here wrong?

//        std::cout<<"[BurstCorrectDetector][transition]winnum: "<<window<<'\n';

        for (auto &i : detect) {
            if (i.window != -1 && window - i.window > winColdThres_) {
                i.window = -1;
            }
            if (i.counters[flag] * lambda_ <= i.counters[flag ^ 1] && i.window != -1) {
                output.emplace_back(Burst<ID_TYPE>(i.window, window, i.id));

                i.window = -1;
            }
            if (i.counters[flag] < burstThreshold_) {
                i.window = -1;
            }
            if (i.counters[flag] >= lambda_ * i.counters[flag ^ 1] && i.counters[flag] >= burstThreshold_) {
                i.window = window;
            }

        }
        flag ^= 1;
        for (auto &i : detect) {
            i.counters[flag] = 0;
        }
    }

    void insert (ID_TYPE id, uint64_t timestamp) {
        if (countBased_) {
            while (last_timestamp + windowSize_ < timestamp) {
                transition(win_cnt++);

                last_timestamp += windowSize_;
            }
        }
        else {
            while (last_timestamp + windowTime_ < timestamp) {
//                std::cout<<"lasttime: "<<last_timestamp<<" time: "<<timestamp<<"\n";
                transition(win_cnt++);
                last_timestamp += windowTime_;
            }

        }
        if (id_index.find(id) == id_index.end()) {
            id_index[id] = detect.size();
            detect.emplace_back(BurstCorrectDetectItem<ID_TYPE>(id));
        }
		assert(id == detect[id_index[id]].id);
        detect[id_index[id]].counters[flag]++;
    }

    std::vector<Burst<ID_TYPE>> query() {
        transition(win_cnt++);
        return output;
    }

private:
    uint8_t flag;
	uint64_t win_cnt;
    std::map<ID_TYPE, int> id_index;
    std::vector<BurstCorrectDetectItem<ID_TYPE>> detect;
    uint64_t last_timestamp;

public:
    std::vector<Burst<ID_TYPE>> output;

};

#endif
