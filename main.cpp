#include <getopt.h>
#include"experiment/memorytest.h"
#include"experiment/ThroughputTest.h"

int main(int argc, char *argv[]){
    /*
    Experiment: ./PBSketch [datasetPath] [length] -[parameter]
        m: memory test
        t: throughput test
    */
    std::string datasetPath;
    int runLength;

    datasetPath = argv[1];
    runLength = atoi(argv[2]);
    int option;
    if((option = getopt(argc, argv, "mt")) != -1){
        switch (option) {
            case 'm':
                MemoryTest::run(datasetPath, runLength);
                break;
            case 't':
                ThroughputTest::run(datasetPath, runLength);
                break;
            default:
                std::cout<<"command error!\n";
                break;
        }
    }

    return 0;
}