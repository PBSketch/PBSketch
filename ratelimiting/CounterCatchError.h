#ifndef BRUSTSKETCHPERIOD_COUNTERCATCHERROR_H
#define BRUSTSKETCHPERIOD_COUNTERCATCHERROR_H

#include <map>
#include <cstdint>
#include <vector>

class CounterCatchError {
    std::map<uint64_t,int> counters;
    std::map<uint64_t, std::vector<uint64_t>[2]> checker;

public:
    CounterCatchError(int _windowSize, int _limitNum);

    void insert(uint64_t _key, uint64_t t);
    void windowOperate(uint64_t t);

public:
    int rejectNum_ = 0;
    int overflowNum_ = 0;

    int windowSize_;
    int limitNum_;
};

#endif //BRUSTSKETCHPERIOD_COUNTERCATCHERROR_H
