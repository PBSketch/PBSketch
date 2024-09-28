//
// Created by kalen on 2024/7/19.
//

#ifndef BRUSTSKETCHPERIOD_THROUGHPUTTEST_H
#define BRUSTSKETCHPERIOD_THROUGHPUTTEST_H

#include <chrono>

#include "../PBSketch/PBSketch.h"
#include "../BurstSketchAddPeriodicSketch/BurstSketchAddPeriodicSketch.h"

class ThroughputTest {
public:
    static BurstSetter getNormalBurstSetter();
    static BurstPartSetter getNormalBurstPartSetter();
    static PeriodicPartSetter getNormalPeriodicPartSetter();
    template<typename ID_TYPE, typename DATA_TYPE, uint32_t CELL_NUM, uint32_t FAILS_NUM>
    static std::chrono::duration<double> testPBSketch(std::string _datasetPath, int _runLength,
                                                      PBSketch<ID_TYPE, DATA_TYPE, CELL_NUM>* _PBSketch, bool _countBased);

    template<typename ID_TYPE, typename DATA_TYPE, uint32_t HASH_NUM, uint32_t CELL_NUM>
    static std::chrono::duration<double>testBaseLine(std::string _datasetPath, int _runLength,
                                                                    BaseLine<ID_TYPE, DATA_TYPE, HASH_NUM, CELL_NUM>* _BaseLine, bool _countBased);

    static void run(std::string _datasetPath, int _runLength);

};


#endif //BRUSTSKETCHPERIOD_THROUGHPUTTEST_H
