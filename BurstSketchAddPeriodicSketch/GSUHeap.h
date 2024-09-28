//
// Created by kalen on 2024/2/16.
//

#ifndef BRUSTSKETCHPERIOD_GSUHEAP_H
#define BRUSTSKETCHPERIOD_GSUHEAP_H

#include "GSUBucket.h"
#include <set>
#include <map>
#include <algorithm>

#include "../util/peridoc.hpp"

class GSUHeap{
public:
    typedef Cell* GSUHeapData_ptr;
private:
    const int containersize_;
    std::vector<GSUHeapData_ptr> container_;

    void swap(int _index1, int _index2);
protected:
    void update(int _index);
    void insert(GSUHeapData_ptr _heapDataPtr);
    void push(GSUHeapData_ptr _heapDataPtr);
    void replace(GSUHeapData_ptr _heapDataPtr);

    void adjustUp(int _index);
    void adjustDown(int _index);

public:
    explicit GSUHeap(int _topk);

    ~GSUHeap();

    void lookup(GSUHeapData_ptr _heapDataPtr);

    std::vector<PBflow> showTop();

public:
    std::multiset<uint64_t> searcher_;
    std::map<GSUHeapData_ptr, int> indexs_;

};

#endif //BRUSTSKETCHPERIOD_WLHEAP_H
