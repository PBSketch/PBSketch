#include <iostream>
#include <getopt.h>
#include "Test.h"



int main(int argc, char *argv[]){
/*
    Experiment: ./PBSketch [datasetPath] [length] -[parameter]
        r: Rate Limit test
        d: Dynamic Buckets amount test
    */
    std::string datasetPath;
    int runLength;
    datasetPath = argv[1];
    runLength = atoi(argv[2]);

    int option;
    if((option = getopt(argc, argv, "rd")) != -1){
        switch (option) {
            case 'r':
                Test::limitNumTestCaida(datasetPath, runLength);
                break;
            case 'd':
                Test::bucketNumTestCaida(datasetPath, runLength);
                break;
            default:
                std::cout<<"command error!\n";
                break;
        }
    }

    return 0;
}
