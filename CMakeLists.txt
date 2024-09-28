cmake_minimum_required(VERSION 3.22)
project(PBSketch)

set(CMAKE_CXX_STANDARD 17)

add_executable(PBSketch
        main.cpp
#        ratelimiting/main.cpp

        common/murmur3.cc
        correctdetector/burstCorrectDetector.h
        correctdetector/periodCorrectDetector.cpp
        correctdetector/periodCorrectDetector.h

        common/burstSetter.h
        PBSketch/burst/burstpartsetter.h
        PBSketch/burst/burstpartsetter.h
        PBSketch/periodicpartsetter.h
        common/burstsetter.cpp
        common/burstsetter.cpp
        correctdetector/PBDetector.h
        experiment/utils/performance.cpp
        experiment/utils/performance.h
        experiment/utils/Comparer.cpp
        experiment/utils/Comparer.h
        experiment/memorytest.cpp
        experiment/memorytest.h


        PBSketch/burst/BOne.h
        PBSketch/burst/BTwo.h
        PBSketch/burst/UpDownBurst.h
        PBSketch/PTwoBucket.h
        PBSketch/pairutil.h
        PBSketch/pairutil.cpp
        PBSketch/periodicPart.h
        PBSketch/PBSketch.h
        PBSketch/Heap.h
        PBSketch/Heap.cpp
        PBSketch/periodichash.h

        BaseLine/BaseLine.h
        BaseLine/GSUBucket.h
        BaseLine/GSUHeap.cpp
        BaseLine/GSUHeap.h
        BaseLine/PeriodicSketch.h
        BaseLine/periodicutil.cpp
        BaseLine/periodicutil.h
        BaseLine/periodichash.h
        BaseLine/periodicpartsetter.h
        BaseLine/burst/BurstSketchStageOne.h
        BaseLine/burst/BurstSketchStageTwo.h
        BaseLine/burst/burstpartsetter.h
        BaseLine/burst/UpDownBurst.h

        experiment/ThroughputTest.cpp
        experiment/ThroughputTest.h
        experiment/utils/Setterbuilder.cpp
        experiment/utils/Setterbuilder.h
        experiment/utils/ParamBuilder.h

        ratelimiting/LeakyBucket.cpp
        ratelimiting/LeakyBucket.h
        ratelimiting/PBSketchLeakyBucket.h
        ratelimiting/CounterCatchError.cpp
        ratelimiting/CounterCatchError.h
        ratelimiting/PBSketchCounterCatchError.h

        ratelimiting/Test.h
        ratelimiting/Test.cpp

)
