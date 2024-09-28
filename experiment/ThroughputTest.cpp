//
// Created by kalen on 2024/7/19.
//

#include "ThroughputTest.h"

#include <fstream>


#include "../correctdetector/PBDetector.h"
#include "utils/Comparer.h"
#include "utils/Setterbuilder.h"
#include "utils/ParamBuilder.h"

void ThroughputTest::run(std::string _datasetPath, int _runLength) {
    const int CellNum = SetterBuilder::NormalCellNum;
    const int Fails = SetterBuilder::NormalFailsNum;

    const int HashNum = 2;

    const int ExperimentNum = ParamBuilder::Throughput::Normal::ExperimentNum;
    PBSketch<uint64_t, uint32_t, CellNum>*PBSketches[ExperimentNum];
    BaseLine<uint64_t, uint32_t, HashNum, CellNum>*BaseLines[ExperimentNum];

    auto memories = ParamBuilder::Throughput::Normal::memories;
    double rate = SetterBuilder::NormalMemoryRate;

    BurstSetter burstSetter = getNormalBurstSetter();
    bool countBased = burstSetter.countBased_;

    for(int index = 0 ;index < ExperimentNum; index++) {

        BurstPartSetter burstPartSetter = getNormalBurstPartSetter();
        PeriodicPartSetter periodicPartSetter = getNormalPeriodicPartSetter();
        periodicPartSetter.periodicMemory_ = (int)( memories[index] * rate * 1000 );
        burstPartSetter.burstMem_ = (int)( memories[index] * (1 - rate) );
        PBSketches[index] = new PBSketch<uint64_t, uint32_t, CellNum>(burstSetter,
                                                                            burstPartSetter,
                                                                            periodicPartSetter);
        BaseLines[index] = new BaseLine<uint64_t, uint32_t, HashNum, CellNum>(burstSetter,
                                                                                                           burstPartSetter,
                                                                                                           periodicPartSetter);
    }


    for(int index = 0 ;index < ExperimentNum; index++) {
        std::cout<<"PBSketch memory: "<< memories[index] <<"\n";
        auto burst_span = testPBSketch<uint64_t, uint32_t, CellNum, Fails>(_datasetPath, _runLength, PBSketches[index], countBased);
        std::cout << "Throughput(Mips):     " << _runLength / (1000 * 1000 * burst_span.count()) << "\n\n";
    }
    for(int index = 0 ;index < ExperimentNum; index++) {
        std::cout<<"BaseLine memory: "<< memories[index] <<"\n";
        auto burst_span = testBaseLine<uint64_t, uint32_t, HashNum, CellNum>(_datasetPath, _runLength, BaseLines[index], countBased);
        std::cout << "Throughput(Mips):     " << _runLength / (1000 * 1000 * burst_span.count()) << "\n\n";
    }

}
BurstSetter ThroughputTest::getNormalBurstSetter() {
    return SetterBuilder::getNormalBurstSetter();
}

BurstPartSetter ThroughputTest::getNormalBurstPartSetter() {
    return SetterBuilder::getNormalBurstPartSetter();
}

PeriodicPartSetter ThroughputTest::getNormalPeriodicPartSetter() {
    return SetterBuilder::getNormalPeriodicPartSetter();
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
std::chrono::duration<double> ThroughputTest::testBaseLine(std::string _datasetPath, int _runLength,
                                                                         BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM> *_BaseLine,
                                                                         bool _countBased) {
    std::ifstream ofs;
    ofs.open(_datasetPath, std::ios::binary);

    auto burst_start = std::chrono::steady_clock::now();
    for (uint32_t i = 0; i < _runLength; ++i) {
        CAIDA_Tuple tuple;
        ofs.read((char *) &tuple, sizeof(CAIDA_Tuple));
        if (_countBased) {
            _BaseLine->insert(tuple.id, i);
        } else {
            _BaseLine->insert(tuple.id, tuple.timestamp);
        }

    }
    auto burst_end = std::chrono::steady_clock::now();
    std::chrono::duration<double> burst_span = std::chrono::duration_cast<std::chrono::duration<double>>(burst_end - burst_start);

    ofs.close();
    return burst_span;
}

template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM, uint32_t FAILS_NUM>
std::chrono::duration<double> ThroughputTest::testPBSketch(std::string _datasetPath, int _runLength,
                                                           PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM> *_PBSketch,
                                                           bool _countBased) {
    std::ifstream ofs;
    ofs.open(_datasetPath, std::ios::binary);

    auto burst_start = std::chrono::steady_clock::now();
    for (uint32_t i = 0; i < _runLength; ++i) {
        CAIDA_Tuple tuple;
        ofs.read((char *) &tuple, sizeof(CAIDA_Tuple));
        if (_countBased) {
            _PBSketch->insert(tuple.id, i);
        } else {
            _PBSketch->insert(tuple.id, tuple.timestamp);
        }

    }
    auto burst_end = std::chrono::steady_clock::now();
    std::chrono::duration<double> burst_span = std::chrono::duration_cast<std::chrono::duration<double>>(burst_end - burst_start);

    ofs.close();
    return burst_span;
}
