//
// Created by kalen on 2024/7/16.
//

#ifndef BRUSTSKETCHPERIOD_MEMORYTEST_H
#define BRUSTSKETCHPERIOD_MEMORYTEST_H

#include "../datasetAnalysis/CAIDADataset.h"
#include <fstream>
#include "utils/Comparer.h"
#include "../PBSketch/PBSketch.h"


class MemoryTest {
public:
    static BurstSetter getNormalBurstSetter();

    static BurstPartSetter getNormalBurstPartSetter();
    static PeriodicPartSetter getNormalPeriodicPartSetter();

    static void run(std::string _datasetPath, int _runLength);


};


#endif //BRUSTSKETCHPERIOD_MEMORYTEST_H
