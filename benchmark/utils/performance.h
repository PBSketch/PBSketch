#ifndef BRUSTSKETCHPERIOD_PERFORMANCE_H
#define BRUSTSKETCHPERIOD_PERFORMANCE_H

#include"string"

class Performance {
public:
    double pr_;
    double rc_;
    double aae_;
    double are_;

    Performance(double _pr, double _rc, double _aae, double _are);
    Performance(int _correct, int _gtNum, int _rsNum, double _aae, double _are);

    Performance();

    double getF1();
    std::string getString();

};


#endif //BRUSTSKETCHPERIOD_PERFORMANCE_H
