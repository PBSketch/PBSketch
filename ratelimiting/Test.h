#ifndef BRUSTSKETCHPERIOD_TEST_H
#define BRUSTSKETCHPERIOD_TEST_H

#include <string>


class Test{
public:
    static const int leakyBucketSize = 100;
    static const int dynamicBucketSize = 40000;

    static const int experimentNum = 5;
    static constexpr int limitNums[experimentNum] = {20,30,40,50,60};

    static const int dynamicBucketNum = 50;

    static const int bucketNumExperimentNum = 9;
    static constexpr int bucketNums[bucketNumExperimentNum] = {1,5,10,20,30,40,50,60,70};

    static const int limitNum = 50;

    static void limitNumTestCaida(std::string _datasetPath, int _runLength);
    static void bucketAmountTestCaida(std::string _datasetPath, int _runLength);
};


#endif //BRUSTSKETCHPERIOD_TEST_H
