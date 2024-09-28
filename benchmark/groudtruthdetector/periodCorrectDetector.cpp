#include "periodCorrectDetector.h"

std::map<PeridocItem, PBflow> &PeriodCorrectDetector::run() {
    GSU.clear();

    for(auto iter : ground_truth){

        uint64_t key = iter.id;

        if(pretimes.find(key)==pretimes.end()){

            pretimes[key] = iter.start_window;

        }else {

            uint64_t period = iter.start_window - pretimes[key];
            period /= delta_;
            pretimes[key] = iter.start_window;

            PeridocItem peridocItem;
            peridocItem.itemKey_ = key;
            peridocItem.peridoc_ = period;

            if(GSU.find(peridocItem) == GSU.end()){
                GSU[peridocItem].peridocItem_ = peridocItem;
                GSU[peridocItem].num_ = 1;

            }else{
                GSU[peridocItem].num_+=1;
            }


        }

    }

    return GSU;
}

PeriodCorrectDetector::PeriodCorrectDetector(std::vector<Burst<uint64_t>> &_gt, double _delta): ground_truth(_gt),delta_(_delta) {}
