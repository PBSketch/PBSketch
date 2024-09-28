#ifndef PERIODCSKETCH_H
#define PERIODCSKETCH_H

#include "GSUBucket.h"
#include "periodicpartsetter.h"

#include "GSUHeap.h"


template <uint32_t HASH_NUM, uint32_t CELL_NUM>
class PeriodicSketch{
public:
    GSUHeap heap;

    double delta_ = 1;
    uint64_t MEMORY;


    PeriodicSketch(uint32_t _MEMORY, int _topk, double ratio = 0.15 , double _delta = 1):heap(_topk), delta_(_delta){

        MEMORY = _MEMORY;

        LIGHT_LENGTH = _MEMORY * ratio / HASH_NUM / sizeof(COUNT_TYPE);
        HEAVY_LENGTH = _MEMORY * (1 - ratio) / sizeof(GSUBucket<CELL_NUM>);

        for(uint32_t i = 0;i < HASH_NUM;++i){
            counter[i] = new TIME_TYPE[LIGHT_LENGTH];
            memset(counter[i], 0, sizeof(TIME_TYPE) * LIGHT_LENGTH);
        }

        buckets = new GSUBucket<CELL_NUM>[HEAVY_LENGTH];

    }
    PeriodicSketch(PeriodicPartSetter _periodicPartSetter):PeriodicSketch(_periodicPartSetter.periodicMemory_, _periodicPartSetter.topK_,
                                                                          _periodicPartSetter.periodicPartRatio_,_periodicPartSetter.delta_){}

    ~PeriodicSketch(){
        for(uint32_t i = 0;i < HASH_NUM;++i)
            delete [] counter[i];

        delete [] buckets;
    }

    void Insert(const PeridocItemPair& item){
        TIME_TYPE minTime = UINT64_MAX;
        auto key = item.item;

        for(uint32_t i = 0;i < HASH_NUM;++i){

            uint32_t position = periodic::periodichash( key, i) % LIGHT_LENGTH;
            minTime = MIN(minTime, counter[i][position]);

            counter[i][position] = item.time;
        }

        auto period = item.time - minTime;
        if(minTime > item.item){
            return;
        }
        period /= delta_;

        PeridocItemPair temp(period, item.item);
        if(temp.time >= 1000)
            return;

        uint32_t position = periodic::periodichash(temp, HASH_NUM) % HEAVY_LENGTH, min_pos = 0;
        COUNT_TYPE min_count = INT32_MAX;

        for(uint32_t j = 0;j < CELL_NUM;++j){
            if(buckets[position].cells[j].item == temp.item && buckets[position].cells[j].interval == temp.time){
                buckets[position].cells[j].count += 1;
                heap.lookup(&buckets[position].cells[j]);
                return;
            }

            if(buckets[position].cells[j].count < min_count){
                min_count = buckets[position].cells[j].count;
                min_pos = j;
            }
        }

        if(periodic::rng() % (2 * min_count - buckets[position].fail + 1) == 0){
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

    PerodicHashMap Report(COUNT_TYPE HIT){
        PerodicHashMap ret;

        for(uint32_t i = 0;i < HEAVY_LENGTH;++i){
            for(uint32_t j = 0;j < CELL_NUM;++j){
                if(buckets[i].cells[j].count > HIT){
                    ret[PeridocItemPair(buckets[i].cells[j].interval, buckets[i].cells[j].item)] = buckets[i].cells[j].count;
                }
            }
        }

        return ret;
    }
    std::vector<PBflow> ReportTop(){

        return heap.showTop();
    }


private:
    uint32_t HEAVY_LENGTH = 0;
    uint32_t LIGHT_LENGTH = 0;

    TIME_TYPE* counter[HASH_NUM]{};
    GSUBucket<CELL_NUM>* buckets;
};

#endif
