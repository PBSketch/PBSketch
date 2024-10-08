#ifndef BRUSTSKETCHPERIOD_SETTERBUILDER_H
#define BRUSTSKETCHPERIOD_SETTERBUILDER_H

#include "../../common//burstSetter.h"
#include "../../PBSketch/burst/burstpartsetter.h"
#include "../../PBSketch/periodicpartsetter.h"

class SetterBuilder {
public:
    static BurstSetter getNormalBurstSetter();

    static BurstPartSetter getNormalBurstPartSetter();
    static PeriodicPartSetter getNormalPeriodicPartSetter();


    static const int NormalCellNum = 7;
    static const int NormalFailsNum = 1;
    constexpr static const double NormalMemoryRate = 0.6;

};


#endif //BRUSTSKETCHPERIOD_SETTERBUILDER_H
