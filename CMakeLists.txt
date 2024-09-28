cmake_minimum_required(VERSION 3.22)
project(PBSketch)

set(CMAKE_CXX_STANDARD 17)

add_executable(PBSketch
        main.cpp
#        ratelimiting/main.cpp

        util/murmur3.cc
        correctdetector/burstCorrectDetector.h
        correctdetector/periodCorrectDetector.cpp
        correctdetector/periodCorrectDetector.h

        util/burstSetter.h
        PBSketch/burst/burstpartsetter.h
        PBSketch/burst/burstpartsetter.h
        PBSketch/periodicpartsetter.h
        util/burstsetter.cpp
        util/burstsetter.cpp
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

        BurstSketchAddPeriodicSketch/BurstSketchAddPeriodicSketch.h
        BurstSketchAddPeriodicSketch/GSUBucket.h
        BurstSketchAddPeriodicSketch/GSUHeap.cpp
        BurstSketchAddPeriodicSketch/GSUHeap.h
        BurstSketchAddPeriodicSketch/PeriodicSketch.h
        BurstSketchAddPeriodicSketch/periodicutil.cpp
        BurstSketchAddPeriodicSketch/periodicutil.h
        BurstSketchAddPeriodicSketch/periodichash.h
        BurstSketchAddPeriodicSketch/periodicpartsetter.h
        BurstSketchAddPeriodicSketch/burst/BurstSketchStageOne.h
        BurstSketchAddPeriodicSketch/burst/BurstSketchStageTwo.h
        BurstSketchAddPeriodicSketch/burst/burstpartsetter.h
        BurstSketchAddPeriodicSketch/burst/UpDownBurst.h

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
