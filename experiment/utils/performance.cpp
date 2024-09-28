//
// Created by kalen on 2024/7/14.
//

#include "performance.h"

Performance::Performance(double _pr, double _rc, double _aae, double _are) {
    pr_ = _pr;
    rc_ = _rc;
    aae_ = _aae;
    are_ = _are;
}

double Performance::getF1() {
    return (2 * pr_ * rc_) / (pr_ + rc_);
}

Performance::Performance(int _correct, int _gtNum, int _rsNum, double _aae, double _are) {
    pr_ = (double)_correct / _rsNum;
    rc_ = (double)_correct / _gtNum;
    aae_ = _aae;
    are_ = _are;
}

std::string Performance::getString() {
    std::string result;
    result += "Precision:            " + std::to_string(pr_) + "\n";
    result += "Recall:               " + std::to_string(rc_) + "\n";
    result += "F1:                   " + std::to_string(getF1()) + "\n";
    result += "aae: " + std::to_string(aae_) + "\n";
    result += "are: " + std::to_string(are_) + "\n";

    return result;
}

Performance::Performance() {
    pr_ = -1;
    rc_ = -1;
    aae_ = -1;
    are_ = -1;
}
