#include "Test.h"

#include <string>
#include <fstream>
#include "LeakyBucket.h"
#include "PBSketchLeakyBucket.h"
#include "PBSketchCounterCatchError.h"
#include "CounterCatchError.h"
#include "../datasetAnalysis/CAIDADataset.h"
#include "../experiment/utils/Setterbuilder.h"

void Test::limitNumTestCaida(std::string _datasetPath, int _runLength) {
    uint64_t mask = 0xFFFFFFFF00000000;
    int memory = 200;
    std::set<uint64_t> ids;
    BurstSetter burstSetter = SetterBuilder::getNormalBurstSetter();

    BurstPartSetter burstPartSetter = SetterBuilder::getNormalBurstPartSetter();
    PeriodicPartSetter periodicPartSetter = SetterBuilder::getNormalPeriodicPartSetter();
    periodicPartSetter.periodicMemory_ = (int)(SetterBuilder::NormalMemoryRate * memory * 1000);
    burstPartSetter.burstMem_ = (int)( (1.0 - SetterBuilder::NormalMemoryRate) * memory);
    burstPartSetter.burstS1Thres_ = burstSetter.burstThreshold_ * 0.15;
    periodicPartSetter.topK_ = 200;

    const int CellNum = SetterBuilder::NormalCellNum;

    PBSketch<uint64_t, uint64_t, CellNum> sketch(burstSetter, burstPartSetter, periodicPartSetter);

    LeakyBucket *leakyBuckets[experimentNum];
    CounterCatchError *counters[experimentNum];
    PBSketchCounterCatchError<uint64_t, uint64_t, CellNum>  *pBSketchCounters[experimentNum];
    PBSketchLeakyBucket<uint64_t, uint64_t, CellNum>  *pBSketchLeakyBuckets[experimentNum];
    for(int i = 0; i < experimentNum; i++){
        leakyBuckets[i] = new LeakyBucket(burstSetter.windowSize_, limitNums[i], leakyBucketSize);
        counters[i] = new CounterCatchError (burstSetter.windowSize_, limitNums[i]);
        pBSketchCounters[i] = new PBSketchCounterCatchError<uint64_t, uint64_t, CellNum> (burstSetter.windowSize_,limitNums[i],dynamicBucketSize,&sketch, dynamicBucketNum);
        pBSketchLeakyBuckets[i] = new PBSketchLeakyBucket<uint64_t, uint64_t, CellNum>  (burstSetter.windowSize_,limitNums[i],leakyBucketSize,dynamicBucketSize,&sketch, dynamicBucketNum);
    }




    std::ifstream ofs;
    ofs.open(_datasetPath  , std::ios::binary);


    for (uint32_t i = 0; i < _runLength; ++i) {
        CAIDA_Tuple tuple;
        ofs.read((char *) &tuple, sizeof(CAIDA_Tuple));

        tuple.id = tuple.id & mask;
        ids.insert(tuple.id);
        for(int experIndex = 0; experIndex < experimentNum; experIndex++){
            counters[experIndex]->insert(tuple.id, i);
            leakyBuckets[experIndex]->insert(tuple.id, i);
            pBSketchCounters[experIndex]->insert(tuple.id, i);
            pBSketchLeakyBuckets[experIndex]->insert(tuple.id, i);
        }

        if(i%40000 == 0) {
            std::cout << "input num: " << std::dec << i << "\n";
        }
    }

    for(int experIndex = 0; experIndex < experimentNum; experIndex++){
        std::cout<<"\n limitNum: "<<limitNums[experIndex]<<"\n\n";

        std::cout<<"counter: \n";
        std::cout<<"counter reject num: "<<counters[experIndex]->rejectNum_<<"\n";
        std::cout<<"counter memory: "<<ids.size() * ( sizeof(uint64_t) + sizeof(uint32_t) ) / 1000 / 1000 << "MB\n";
        std::cout<<"overflowNum_: "<<counters[experIndex]->overflowNum_<<"\n";

        std::cout<<"PBSketch counter: \n";
        std::cout<<"counter reject num: "<<pBSketchCounters[experIndex]->rejectNum_<<"\n";
        std::cout<<"counter memory: "<<ids.size() * ( sizeof(uint64_t) + sizeof(uint32_t) ) / 1000 / 1000 << "MB\n";
        std::cout<<"dynamicBucket memory: "<< pBSketchCounters[experIndex]->maxn * sizeof(uint64_t)  * pBSketchCounters[experIndex]->maxBucketSize_ / 1000 / 1000 << "MB\n";
        std::cout<<"PBSketch memory: "<< (double)memory / 1000<<"MB\n";
        std::cout<<"overflowNum_: "<<pBSketchCounters[experIndex]->overflowNum_<<"\n";

        std::cout<<"LeakyBucket: \n";
        std::cout<<"leakyBucket reject num: "<<leakyBuckets[experIndex]->rejectNum_<<"\n";
        std::cout<<"bucket memory: "<< (double)ids.size() * sizeof(uint64_t) * leakyBuckets[experIndex]->maxBucketSize_ / 1000 / 1000 << "MB\n";

        std::cout<<"PBSketch LeakyBucket: \n";
        std::cout<<"pBSketchLeakyBucket reject num: "<<pBSketchLeakyBuckets[experIndex]->rejectNum_<<"\n";
        std::cout<<"bucket memory: "<< (double)ids.size() * sizeof(uint64_t) * pBSketchLeakyBuckets[experIndex]->maxBucketSize_ / 1000 / 1000 << "MB\n";
        std::cout<<"dy bucket memory: "<< (double)pBSketchLeakyBuckets[experIndex]->maxn * sizeof(uint64_t)  * pBSketchLeakyBuckets[experIndex]->dynamicBucketCapacity_ / 1000 / 1000 << "MB\n";
        std::cout<<"PBSketch memory: "<< (double)memory / 1000<<"MB\n";
    }
}

void Test::bucketNumTestCaida(std::string _datasetPath, int _runLength) {
    uint64_t mask = 0xFFFFFFFF00000000;
    int memory = 200;
    std::set<uint64_t> ids;

    BurstSetter burstSetter = SetterBuilder::getNormalBurstSetter();

    BurstPartSetter burstPartSetter = SetterBuilder::getNormalBurstPartSetter();
    PeriodicPartSetter periodicPartSetter = SetterBuilder::getNormalPeriodicPartSetter();
    periodicPartSetter.periodicMemory_ = (int)(SetterBuilder::NormalMemoryRate * memory * 1000);
    burstPartSetter.burstMem_ = (int)( (1.0 - SetterBuilder::NormalMemoryRate) * memory);
    burstPartSetter.burstS1Thres_ = burstSetter.burstThreshold_ * 0.15;
    periodicPartSetter.topK_ = 200;

    const int CellNum = SetterBuilder::NormalCellNum;


    PBSketch<uint64_t, uint64_t, CellNum> sketch(burstSetter, burstPartSetter, periodicPartSetter);

    PBSketchCounterCatchError<uint64_t, uint64_t, CellNum>  *pBSketchCounters[bucketNumExperimentNum];
    PBSketchLeakyBucket<uint64_t, uint64_t, CellNum>  *pBSketchLeakyBuckets[bucketNumExperimentNum];
    for(int i = 0; i < bucketNumExperimentNum; i++){
        pBSketchCounters[i] = new PBSketchCounterCatchError<uint64_t, uint64_t, CellNum> (burstSetter.windowSize_,limitNum,dynamicBucketSize,&sketch, bucketNums[i]);
        pBSketchLeakyBuckets[i] = new PBSketchLeakyBucket<uint64_t, uint64_t, CellNum>  (burstSetter.windowSize_,limitNum,leakyBucketSize,dynamicBucketSize,&sketch, bucketNums[i]);
    }




    std::ifstream ofs;
    ofs.open(_datasetPath  , std::ios::binary);


    for (uint32_t i = 0; i < _runLength; ++i) {
        CAIDA_Tuple tuple;
        ofs.read((char *) &tuple, sizeof(CAIDA_Tuple));

        tuple.id = tuple.id & mask;
        ids.insert(tuple.id);

        for(int experIndex = 0; experIndex < bucketNumExperimentNum; experIndex++){
            pBSketchCounters[experIndex]->insert(tuple.id, i);
            pBSketchLeakyBuckets[experIndex]->insert(tuple.id, i);
        }

        if(i%40000 == 0) {
            std::cout << "input num: " << std::dec << i << "\n";
        }
    }

    for(int experIndex = 0; experIndex < bucketNumExperimentNum; experIndex++){
        std::cout<<"\nbucket Num: "<<bucketNums[experIndex]<<"\n\n";

        std::cout<<"PBSketch counter: \n";
        std::cout<<"counter reject num: "<<pBSketchCounters[experIndex]->rejectNum_<<"\n";
        std::cout<<"counter memory: "<<ids.size() * ( sizeof(uint64_t) + sizeof(uint32_t) ) / 1000 / 1000 << "MB\n";
        std::cout<<"dynamicBucket memory: "<< pBSketchCounters[experIndex]->maxn * sizeof(uint64_t)  * pBSketchCounters[experIndex]->maxBucketSize_ / 1000 / 1000 << "MB\n";
        std::cout<<"PBSketch memory: "<< (double)memory / 1000<<"MB\n";
        std::cout<<"boundary problem Num_: "<<pBSketchCounters[experIndex]->overflowNum_<<"\n";

        std::cout<<"PBSketch LeakyBucket: \n";
        std::cout<<"pBSketchLeakyBucket reject num: "<<pBSketchLeakyBuckets[experIndex]->rejectNum_<<"\n";
        std::cout<<"bucket memory: "<< (double)ids.size() * sizeof(uint64_t) * pBSketchLeakyBuckets[experIndex]->maxBucketSize_ / 1000 / 1000 << "MB\n";
        std::cout<<"dy bucket memory: "<< (double)pBSketchLeakyBuckets[experIndex]->maxn * sizeof(uint64_t)  * pBSketchLeakyBuckets[experIndex]->dynamicBucketCapacity_ / 1000 / 1000 << "MB\n";
        std::cout<<"PBSketch memory: "<< (double)memory / 1000<<"MB\n";
    }
}
