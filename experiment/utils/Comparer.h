//
// Created by kalen on 2024/7/14.
//

#ifndef BRUSTSKETCHPERIOD_COMPARER_H
#define BRUSTSKETCHPERIOD_COMPARER_H

#include "performance.h"
#include "../../util/peridoc.hpp"
#include <map>

class Comparer {
public:
    static Performance compare(std::map<PeridocItem, PBflow> &result, std::map<PeridocItem, PBflow> &ground_truth);
    static Performance compareTop(std::map<PeridocItem, PBflow> &result, std::map<PeridocItem, PBflow> &ground_truth, int _topK);
};


#endif //BRUSTSKETCHPERIOD_COMPARER_H
