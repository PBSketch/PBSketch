#ifndef BRUSTSKETCHPERIOD_PBDETECTOR_H
#define BRUSTSKETCHPERIOD_PBDETECTOR_H

#include "burstCorrectDetector.h"
#include "periodCorrectDetector.h"

#include "../datasetAnalysis/CAIDADataset.h"

template<typename ID_TYPE>
class PBDetector {
public:
    BurstCorrectDetector<ID_TYPE>* burstCorrectDetectorPtr = nullptr;
    PeriodCorrectDetector* periodCorrectDetectorPtr = nullptr;
    double delta_;
    bool countBased_;

    PBDetector(BurstSetter _burstSetter, double _delta = 1): delta_(_delta){
        burstCorrectDetectorPtr = new BurstCorrectDetector<ID_TYPE>(_burstSetter);
        countBased_ = _burstSetter.countBased_;
    }
    ~PBDetector(){
        delete burstCorrectDetectorPtr;
        if(periodCorrectDetectorPtr != nullptr){
            delete periodCorrectDetectorPtr;
        }
    }
    std::map<PeridocItem, PBflow> runCaida(std::string _datasetPath, uint32_t _runLength){
        std::ifstream ofs;
        ofs.open( _datasetPath, std::ios::binary);

        for (uint32_t i = 0; i < _runLength; ++i) {
            CAIDA_Tuple tuple;
            ofs.read((char *) &tuple, sizeof(CAIDA_Tuple));

            if (countBased_) {
                burstCorrectDetectorPtr->insert(tuple.id, i);
            } else {
                burstCorrectDetectorPtr->insert(tuple.id, tuple.timestamp);
            }

            if(i%40000 == 0){
                std::cout<<"count: "<<i<<"/"<<_runLength<<"\n";
            }
        }
        ofs.close();

        std::vector<Burst<uint64_t>> ground_truth = burstCorrectDetectorPtr->query();

        periodCorrectDetectorPtr = new PeriodCorrectDetector(ground_truth, delta_);

        return periodCorrectDetectorPtr->run();

    }
};


#endif //BRUSTSKETCHPERIOD_PBDETECTOR_H
