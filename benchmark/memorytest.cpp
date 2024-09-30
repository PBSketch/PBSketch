//
// Created by kalen on 2024/7/16.
//

#include "memorytest.h"
#include "groudtruthdetector/PBDetector.h"
#include "../BaseLine//BaseLine.h"
#include "utils/Setterbuilder.h"
#include "utils/ParamBuilder.h"

PeriodicPartSetter MemoryTest::getNormalPeriodicPartSetter() {
    return SetterBuilder::getNormalPeriodicPartSetter();
}

BurstPartSetter MemoryTest::getNormalBurstPartSetter() {
    return SetterBuilder::getNormalBurstPartSetter();
}

BurstSetter MemoryTest::getNormalBurstSetter() {
   return SetterBuilder::getNormalBurstSetter();
}

void MemoryTest::run(std::string _datasetPath, int _runLength) {
    std::cout << "memory test \n";


    const int CellNum = SetterBuilder::NormalCellNum;

    const int HashNum = 2;

    const int ExperimentNum = ParamBuilder::Memory::Normal::ExperimentNum;
    PBSketch<uint64_t, uint32_t, CellNum>*PBSketchs[ExperimentNum];
    BaseLine<uint64_t, uint32_t, HashNum, CellNum>*BaseLines[ExperimentNum];
    std::map<PeridocItem, PBflow> PBResults[ExperimentNum];
    std::map<PeridocItem, PBflow> BAddPResults[ExperimentNum];

    auto memories = ParamBuilder::Memory::Normal::memories;
    double rate = SetterBuilder::NormalMemoryRate;



    BurstSetter burstSetter = getNormalBurstSetter();
    PBDetector<uint64_t> pBDetector(burstSetter, getNormalPeriodicPartSetter().delta_);

    bool countBased = burstSetter.countBased_;
    int topK = 100;

    for(int index = 0 ;index < ExperimentNum; index++) {

        std::cout << "memory: " << memories[index] << "\n";

        BurstPartSetter burstPartSetter = getNormalBurstPartSetter();
        PeriodicPartSetter periodicPartSetter = getNormalPeriodicPartSetter();
        periodicPartSetter.periodicMemory_ = (int)( memories[index] * rate * 1000 );
        burstPartSetter.burstMem_ = (int)( memories[index] * (1 - rate) );
        PBSketchs[index] = new PBSketch<uint64_t, uint32_t, CellNum>(burstSetter,
                                                                            burstPartSetter,
                                                                            periodicPartSetter);
        BaseLines[index] = new BaseLine<uint64_t, uint32_t, HashNum, CellNum>(burstSetter,
                                                                            burstPartSetter,
                                                                            periodicPartSetter);

        topK = periodicPartSetter.topK_;
    }

    std::ifstream ofs;
    ofs.open(_datasetPath, std::ios::binary);

    for (uint32_t i = 0; i < _runLength; ++i) {
        CAIDA_Tuple tuple;
        ofs.read((char *) &tuple, sizeof(CAIDA_Tuple));
        if (countBased) {
            for(int index = 0 ;index < ExperimentNum; index++){
                PBSketchs[index]->insert(tuple.id, i);
                BaseLines[index]->insert(tuple.id, i);
            }

        } else {
            for(int index = 0 ;index < ExperimentNum; index++){
                PBSketchs[index]->insert(tuple.id, tuple.timestamp);
                BaseLines[index]->insert(tuple.id, tuple.timestamp);
            }
        }

        if(i%400000 == 0){
            std::cout<<"SKetch count: "<<i<<"/"<<_runLength<<"\n";
        }
    }
    ofs.close();

   for(int index = 0 ;index < ExperimentNum; index++) {
       auto result = PBSketchs[index]->Report(0);
       for (auto &iter: result) {

           PBflow pBflow;
           pBflow.num_ = iter.second;
           pBflow.peridocItem_.itemKey_ = iter.first.item;
           pBflow.peridocItem_.peridoc_ = iter.first.time;

           PBResults[index][pBflow.peridocItem_] = pBflow;
       }

   }
    for(int index = 0 ;index < ExperimentNum; index++) {
        auto result = BaseLines[index]->Report(0);
        for (auto &iter: result) {

            PBflow pBflow;
            pBflow.num_ = iter.second;
            pBflow.peridocItem_.itemKey_ = iter.first.item;
            pBflow.peridocItem_.peridoc_ = iter.first.time;

            BAddPResults[index][pBflow.peridocItem_] = pBflow;
        }

    }


    auto gt = pBDetector.runCaida(_datasetPath , _runLength);

    std::cout<<"PBSketch: \n";
    for(int index = 0 ;index < ExperimentNum; index++) {
        std::cout<<"memory: "<< memories[index] <<"\n";

        auto perform = Comparer::compareTop(PBResults[index], gt, topK);
        std::cout << perform.getString();

        std::cout<<"\n";
        delete PBSketchs[index];

    }
    std::cout<<"\nBaseLine: \n";
    for(int index = 0 ;index < ExperimentNum; index++) {
        std::cout<<"memory: "<< memories[index] <<"\n";

        auto perform = Comparer::compareTop(BAddPResults[index], gt, topK);
        std::cout << perform.getString();

        std::cout<<"\n";
        delete BaseLines[index];

    }
}
