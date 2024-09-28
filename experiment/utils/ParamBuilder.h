//
// Created by kalen on 2024/7/24.
//

#ifndef BRUSTSKETCHPERIOD_PARAMBUILDER_H
#define BRUSTSKETCHPERIOD_PARAMBUILDER_H


class ParamBuilder {
public:
    class Throughput{
    public:
        class Normal{
        public:
            static const int ExperimentNum = 5;
            static constexpr double memories[ExperimentNum] = {50, 75, 100, 125, 150};

        };
    };
    class Memory{
    public:
        class Normal{
        public:
            static const int ExperimentNum = 5;
            static constexpr double memories[ExperimentNum] = {50, 75, 100, 125, 150};

        };
    };


};


#endif //BRUSTSKETCHPERIOD_PARAMBUILDER_H
