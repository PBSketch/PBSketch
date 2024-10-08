#ifndef BRUSTSKETCHPERIOD_COMPARER_H
#define BRUSTSKETCHPERIOD_COMPARER_H

#include "performance.h"
#include "../../common/peridoc.hpp"
#include <map>

class Comparer {
public:
    static Performance compare(std::map<PeridocItem, PBflow> &result, std::map<PeridocItem, PBflow> &ground_truth);
    static Performance compareTop(std::map<PeridocItem, PBflow> &result, std::map<PeridocItem, PBflow> &ground_truth, int _topK);
};


#endif //BRUSTSKETCHPERIOD_COMPARER_H
