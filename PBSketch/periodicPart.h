#ifndef BRUSTSKETCHPERIOD_PERIOICPART_H
#define BRUSTSKETCHPERIOD_PERIOICPART_H

#include "PTwoBucket.h"
#include "periodicpartsetter.h"

#include "Heap.h"



template <uint32_t CELL_NUM>
class PeriodicPart{
public:
    Heap heap;
    std::vector<uint64_t> POneKeys;
    std::vector<uint32_t> POneTimes;
    int POneSize_ = 0;

    double delta_ = 1;
    uint64_t MEMORY;

    const int HASH_SEED = 1;

private:
    uint32_t HEAVY_LENGTH = 0;

    PTwoBucket<CELL_NUM>* buckets;

public:
//    PeriodicPart(uint32_t _MEMORY, int _topk, double ratio = 0.15 , double _delta = 1): heap(_topk), delta_(_delta){
    PeriodicPart(uint32_t _MEMORY, int _topk, double ratio = 0.15 , double _delta = 1): heap(_topk){

        if(_delta == 0){
            delta_ = 1;
        }else{
            delta_ = 2 * _delta;
        }

        MEMORY = _MEMORY;

        POneSize_ = _MEMORY  * ratio / (sizeof (TIME_TYPE) + sizeof (uint64_t));
        HEAVY_LENGTH = _MEMORY * (1 - ratio) / sizeof(PTwoBucket<CELL_NUM>);
        buckets = new PTwoBucket<CELL_NUM>[HEAVY_LENGTH];


    }
    PeriodicPart(PeriodicPartSetter _periodicPartSetter):PeriodicPart(_periodicPartSetter.periodicMemory_, _periodicPartSetter.topK_,
                                                                          _periodicPartSetter.periodicPartRatio_,_periodicPartSetter.delta_){}

    ~PeriodicPart(){
        delete [] buckets;
    }

    void Insert(const PTwoItemPair& item){
        //P1
        TIME_TYPE minTime = UINT64_MAX;
        bool POneFlag = false;
        int pos = 0;
        for(int i = 0; i < POneKeys.size();i++){
            if(POneKeys[i] == item.item){
                minTime = POneTimes[i];
                POneTimes[i]=item.time;
                POneFlag = true;
                break;
            }
            if(POneTimes[i] < POneTimes[pos]){
                pos = i;
            }
        }
        if(!POneFlag){
            if(POneKeys.size() >=POneSize_){
                POneKeys[pos] = item.item;
                POneTimes[pos] = item.time;

            }else{
                POneKeys.push_back(item.item);
                POneTimes.push_back(item.time);

            }
            return;
        }


        auto period = item.time - minTime;
        if(minTime > item.item){
            return;
        }
        period /= delta_;

        PTwoItemPair temp(period, item.item);
        if(temp.time >= 1000)
            return;

        //P Two
        uint32_t position = periodic::periodichash(temp, HASH_SEED) % HEAVY_LENGTH, min_pos = 0;
        COUNT_TYPE min_count = INT32_MAX;

        for(uint32_t j = 0;j < CELL_NUM;++j){
            if(buckets[position].cells[j].item == temp.item &&
                    buckets[position].cells[j].interval == temp.time){
                buckets[position].cells[j].count += 1;
                heap.lookup(&buckets[position].cells[j]);
                return;
            }

            if(buckets[position].cells[j].count < min_count){
                min_count = buckets[position].cells[j].count;
                min_pos = j;
            }
        }
        auto _x = 2 * min_count - abs(buckets[position].fail);
        if(  _x <= 0 || periodic::rng() % _x == 0){
            auto replace = buckets[position].cells[min_pos];
            buckets[position].cells[min_pos].item = temp.item;
            buckets[position].cells[min_pos].interval = temp.time;

            if(min_count == 0){
                buckets[position].cells[min_pos].count = 1;
            }
            else{

                    buckets[position].cells[min_pos].count += buckets[position].fail / min_count;

                if(heap.indexs_.find(&buckets[position].cells[min_pos]) != heap.indexs_.end()){
                    heap.searcher_.erase(heap.searcher_.find(replace.item));
                    heap.searcher_.insert(temp.item);
                }
            }

                buckets[position].fail = 0;


            heap.lookup( &buckets[position].cells[min_pos]);
        }
        else{
                buckets[position].fail += 1;
        }
    }

    PTwoItemPairHashMap Report(COUNT_TYPE HIT){
        PTwoItemPairHashMap ret;

        for(uint32_t i = 0;i < HEAVY_LENGTH;++i){
            for(uint32_t j = 0;j < CELL_NUM;++j){
                if(buckets[i].cells[j].count > HIT){
                    ret[PTwoItemPair(buckets[i].cells[j].interval,
                            buckets[i].cells[j].item)]
                    = buckets[i].cells[j].count;
                }
            }
        }

        return ret;
    }
    std::vector<PBflow> ReportTop(){

        return heap.showTop();
    }

};

#endif
