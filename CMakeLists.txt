cmake_minimum_required(VERSION 3.22)
project(PBSketch)

set(CMAKE_CXX_STANDARD 17)

add_executable(PBSketch
        main.cpp

        common/murmur3.cc
        benchmark/groudtruthdetector/burstCorrectDetector.h
        benchmark/groudtruthdetector/periodCorrectDetector.cpp
        benchmark/groudtruthdetector/periodCorrectDetector.h

        common/burstSetter.h
        PBSketch/burst/burstpartsetter.h
        PBSketch/burst/burstpartsetter.h
        PBSketch/periodicpartsetter.h
        common/burstsetter.cpp
        common/burstsetter.cpp
        benchmark/groudtruthdetector/PBDetector.h
        benchmark/utils/performance.cpp
        benchmark/utils/performance.h
        benchmark/utils/Comparer.cpp
        benchmark/utils/Comparer.h
        benchmark/memorytest.cpp
        benchmark/memorytest.h


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

        benchmark/ThroughputTest.cpp
        benchmark/ThroughputTest.h
        benchmark/utils/Setterbuilder.cpp
        benchmark/utils/Setterbuilder.h
        benchmark/utils/ParamBuilder.h

        ratelimiting/LeakyBucket.cpp
        ratelimiting/LeakyBucket.h
        ratelimiting/PBSketchLeakyBucket.h
        ratelimiting/CounterCatchError.cpp
        ratelimiting/CounterCatchError.h
        ratelimiting/PBSketchCounterCatchError.h

        ratelimiting/Test.h
        ratelimiting/Test.cpp

)
