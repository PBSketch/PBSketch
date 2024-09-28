//
// Created by kalen on 2024/2/16.
//

#include "GSUHeap.h"
#include <algorithm>


void GSUHeap::push(GSUHeap::GSUHeapData_ptr _heapDataPtr) {

    indexs_[_heapDataPtr] = container_.size();
    container_.push_back(_heapDataPtr);
    searcher_.insert(_heapDataPtr->item);

    adjustUp(indexs_[_heapDataPtr]);


}

void GSUHeap::replace(GSUHeap::GSUHeapData_ptr _heapDataPtr) {

    if(_heapDataPtr->count > container_[0]->count ){

        indexs_.erase(indexs_.find(container_[0]));
        indexs_[_heapDataPtr] = 0;

        searcher_.erase(searcher_.find(container_[0]->item));
        searcher_.insert(_heapDataPtr->item);

        container_[0] = _heapDataPtr;

        adjustDown(0);

    }

}

void GSUHeap::adjustUp(int _index) {
    if(_index == 0){
        return;
    }

    int parent = (_index - 1) / 2;
    int left = parent * 2 + 1;
    int right = parent * 2 + 2;

    if(right >= container_.size() ){

        if(left < container_.size() && container_[left]->count < container_[parent]->count){
            swap(left, parent);
            adjustUp(parent);
        }

    }else{
        if(container_[left]->count < container_[parent]->count || container_[right]->count < container_[parent]->count){

            if(container_[left]->count < container_[right]->count){
                swap(left, parent);
            }else{
                swap(right, parent);
            }

            adjustUp(parent);
        }
    }
}

void GSUHeap::adjustDown(int _index) {
    if(_index >= container_.size()){
        return;
    }

    int parent = _index;
    int left = parent * 2 + 1;
    int right = parent * 2 + 2;

    if(right >= container_.size() ){

        if(left < container_.size() && container_[left]->count < container_[parent]->count){
            swap(left, parent);
            adjustDown(left);
        }

    }else{

        if(container_[left]->count < container_[parent]->count || container_[right]->count < container_[parent]->count){

            if(container_[left]->count < container_[right]->count){
                swap(left, parent);
                adjustDown(left);

            }else{
                swap(right, parent);
                adjustDown(right);
            }

        }
    }
}

void GSUHeap::insert(GSUHeap::GSUHeapData_ptr _heapDataPtr) {
    if(container_.size() < containersize_){
        push(_heapDataPtr);
    }else{
        replace(_heapDataPtr);
    }

}

void GSUHeap::update(int _index) {
    adjustDown(_index);
    adjustUp(_index);
}

void GSUHeap::swap(int _index1, int _index2) {
    GSUHeapData_ptr temp = container_[_index1];
    container_[_index1] = container_[_index2];
    container_[_index2] = temp;


    indexs_[container_[_index1]] = _index1;
    indexs_[container_[_index2]] = _index2;
}

GSUHeap::GSUHeap(int _topk) : containersize_(_topk){

}

void GSUHeap::lookup(GSUHeap::GSUHeapData_ptr _heapDataPtr) {

    if(indexs_.find(_heapDataPtr) != indexs_.end() ){

        update(indexs_[_heapDataPtr]);
    }else{
        insert(_heapDataPtr);
    }
}

std::vector<PBflow> GSUHeap::showTop() {
    std::vector<PBflow> result;

    for(int i = 0; i < container_.size(); i++){

        auto cell_ptr = container_[i];

        PBflow pBflow;
        pBflow.num_ = cell_ptr->count;
        pBflow.peridocItem_.itemKey_ = cell_ptr->item;
        pBflow.peridocItem_.peridoc_ = cell_ptr->interval;

        result.push_back(pBflow);
    }

    std::sort(result.begin(), result.end());

    return result;
}

GSUHeap::~GSUHeap() {
    searcher_.clear();
    indexs_.clear();
}
