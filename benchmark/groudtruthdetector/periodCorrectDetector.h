#ifndef BRUSTSKETCHPERIOD_PERIODCORRECTDETECTOR_H
#define BRUSTSKETCHPERIOD_PERIODCORRECTDETECTOR_H


#include <map>
#include "../../common/peridoc.hpp"
#include "../../common/Burst.h"




class PeriodCorrectDetector {
public:
    std::map<PeridocItem, PBflow> GSU;
    std::map<PeridocItem, PBflow>& run();
    PeriodCorrectDetector(std::vector<Burst<uint64_t>> &_gt, double _delta = 1);

public:
    double delta_;
private:
    std::map<uint64_t, uint32_t> pretimes;
    std::vector<Burst<uint64_t>>&ground_truth;

};


#endif //BRUSTSKETCHPERIOD_PERIODCORRECTDETECTOR_H
