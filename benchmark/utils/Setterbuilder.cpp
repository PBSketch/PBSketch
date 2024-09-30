//
// Created by kalen on 2024/7/20.
//

#include "Setterbuilder.h"

BurstSetter SetterBuilder::getNormalBurstSetter() {
    bool countBased = true;
    int windowSize = 40000;
    int winColdThres = 10;
    int lambda = 2;
    int BurstThreshold = 50;

    BurstSetter burstSetter(countBased, windowSize, winColdThres, lambda, BurstThreshold);

    return burstSetter;
}

BurstPartSetter SetterBuilder::getNormalBurstPartSetter() {
    int hashNum = 2;
    int stage2BucketSize = 4;
    int burstMemory = 50;//(KB)
    double stageRatio = 0.6;
    int runningTrackThres = 15;
    int kickMaxNum = 100;

    BurstPartSetter burstPartSetter(hashNum, stage2BucketSize, burstMemory, stageRatio, runningTrackThres, kickMaxNum);
    return burstPartSetter;
}

PeriodicPartSetter SetterBuilder::getNormalPeriodicPartSetter() {
    int periodicMemory = 100000;//(B)
    double CMGSUratio = 0.1;
    double delta = 0;
    int topK = 100;

    PeriodicPartSetter periodicPartSetter(periodicMemory, CMGSUratio, delta, topK);
    return periodicPartSetter;
}