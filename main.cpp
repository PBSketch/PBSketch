#include <getopt.h>
#include"benchmark/memorytest.h"
#include"benchmark/ThroughputTest.h"
#include "ratelimiting/Test.h"

int main(int argc, char *argv[]){
    /*
    Experiment: ./PBSketch [datasetPath] [length] -[parameter]
        m: memory test
        t: throughput test
        r: Rate Limit teat-Limit num test
        d: Rate Limit teat-Dynamic Buckets amount test
    */
    std::string datasetPath;
    int runLength;

    datasetPath = argv[1];
    runLength = atoi(argv[2]);
    int option;
    if((option = getopt(argc, argv, "mtrd")) != -1){
        switch (option) {
            case 'm':
                MemoryTest::run(datasetPath, runLength);
                break;
            case 't':
                ThroughputTest::run(datasetPath, runLength);
                break;
            case 'r':
                Test::limitNumTestCaida(datasetPath, runLength);
                break;
            case 'd':
                Test::bucketAmountTestCaida(datasetPath, runLength);
                break;
            default:
                std::cout<<"command error!\n";
                break;
        }
    }

    return 0;
}