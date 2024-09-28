//
// Created by kalen on 2024/2/16.
//

#ifndef BRUSTSKETCHPERIOD_HEAP_H
#define BRUSTSKETCHPERIOD_HEAP_H

#include "PTwoBucket.h"
#include <set>
#include <map>
#include <algorithm>

#include "../util/peridoc.hpp"

class Heap{
public:
    typedef PTwoCell* HeapData_ptr;
private:
    const int containersize_;
    std::vector<HeapData_ptr> container_;

    void swap(int _index1, int _index2);
protected:
    void update(int _index);
    void insert(HeapData_ptr _heapDataPtr);
    void push(HeapData_ptr _heapDataPtr);
    void replace(HeapData_ptr _heapDataPtr);

    void adjustUp(int _index);
    void adjustDown(int _index);

public:
    explicit Heap(int _topk);

    ~Heap();

    void lookup(HeapData_ptr _heapDataPtr);

    std::vector<PBflow> showTop();

public:
    std::multiset<uint64_t> searcher_;
    std::map<HeapData_ptr, int> indexs_;

};

#endif //BRUSTSKETCHPERIOD_HEAP_H
