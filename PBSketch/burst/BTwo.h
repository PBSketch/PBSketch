#ifndef BRUSTSKETCHPERIOD_BURSTPASTSTAGETWO_H
#define BRUSTSKETCHPERIOD_BURSTPASTSTAGETWO_H

#include <cstdint>
#include <vector>
#include "UpDownBurst.h"
#include <limits>

template<typename ID_TYPE, typename DATA_TYPE>
class BTwo {

public:
    BTwo(int b_num, int b_size, uint32_t s1_thres, uint32_t s2_thres,int _kickMaxNum);

    ~BTwo();

    bool lookup(ID_TYPE id);

    bool insert(ID_TYPE id, int32_t window, DATA_TYPE count, const std::multiset<uint64_t>&_searcher);

    void window_transition(uint32_t window, const std::multiset<uint64_t> &_searcher);


private:
    struct Slot {
        ID_TYPE id;
        DATA_TYPE counters[2];
        int32_t window;
    };

    uint32_t stage_one_thres, stage_two_thres;
    int bucket_num, bucket_size;
    uint8_t flag;
    Slot **slots;

public:
    std::vector<UpDownBurst<ID_TYPE>> results;

    int win_cold_thres;
    int lambda;
    int HashNum = 2;
    int kickMaxNum;

    void kick(Slot slot, const std::multiset<uint64_t> &_searcher, int kickNum, int ignoreIndex);
};

template<typename ID_TYPE, typename DATA_TYPE>
bool BTwo<ID_TYPE, DATA_TYPE>::insert(ID_TYPE id, int32_t window, DATA_TYPE count,
                                                      const std::multiset<uint64_t> &_searcher) {
    for(int index = 0; index < HashNum; index++){
        int bucket_pos = (hash(id, 32 + index) % (bucket_num / HashNum) ) + (index * (bucket_num / HashNum) );
        int slot_pos = 0;
        DATA_TYPE min_counter = std::numeric_limits<DATA_TYPE>::max();
        bool non_bursting = false;
        for (int i = 0; i < bucket_size; ++i) {
            if (slots[bucket_pos][i].id == 0) { // empty
                slot_pos = i;
                break;
            }
            if (slots[bucket_pos][i].window == -1) {
                if (!non_bursting) {
                    non_bursting = true;
                    min_counter = std::numeric_limits<DATA_TYPE>::max();
                }
                if (slots[bucket_pos][i].counters[flag] < min_counter) {
                    min_counter = slots[bucket_pos][i].counters[flag];
                    slot_pos = i;
                }
            }
            else if (!non_bursting) {
                if (slots[bucket_pos][i].counters[flag] < min_counter){
                    min_counter = slots[bucket_pos][i].counters[flag];
                    slot_pos = i;
                }
            }
        }

        if (slots[bucket_pos][slot_pos].id == 0) {
            slots[bucket_pos][slot_pos].id = id;
            slots[bucket_pos][slot_pos].window = -1;
            slots[bucket_pos][slot_pos].counters[flag] = count;
            slots[bucket_pos][slot_pos].counters[flag ^ 1] = 0;

            return true;
        }
        if( count >= slots[bucket_pos][slot_pos].counters[flag] && _searcher.find( slots[bucket_pos][slot_pos].id) == _searcher.end() )
        {  //evict
            auto oldId = slots[bucket_pos][slot_pos].id;
            auto oldCount = slots[bucket_pos][slot_pos].counters[flag];

            Slot oldSlot;
            oldSlot.id = oldId;
            oldSlot.counters[flag] = oldCount;
            oldSlot.counters[flag ^ 1] = slots[bucket_pos][slot_pos].counters[flag ^ 1];
            oldSlot.window = slots[bucket_pos][slot_pos].window;

            slots[bucket_pos][slot_pos].id = id;
            slots[bucket_pos][slot_pos].window = -1;
            slots[bucket_pos][slot_pos].counters[flag] = count;
            slots[bucket_pos][slot_pos].counters[flag ^ 1] = 0;

            kick(oldSlot, _searcher, kickMaxNum, index);

            return true;
        }
    }


    return false;
}

template<typename ID_TYPE, typename DATA_TYPE>
    void BTwo<ID_TYPE, DATA_TYPE>::kick(Slot slot, const std::multiset<uint64_t> &_searcher, int kickNum, int ignoreIndex) {
    if(kickNum <= 0){
        return;
    }else{
        kickNum --;
    }
    ID_TYPE id = slot.id;
    DATA_TYPE count = slot.counters[flag];
    for(int index = 0; index < HashNum; index++){
        if(index == ignoreIndex){
            continue;
        }

        int bucket_pos = (hash(id, 32 + index) % (bucket_num / HashNum) ) + (index * (bucket_num / HashNum) );

        int slot_pos = 0;
        DATA_TYPE min_counter = std::numeric_limits<DATA_TYPE>::max();
        bool non_bursting = false;

        for (int i = 0; i < bucket_size; ++i) {
            if (slots[bucket_pos][i].id == 0) {
                slot_pos = i;
                break;
            }
            if (slots[bucket_pos][i].window == -1) {
                if (!non_bursting) {
                    non_bursting = true;
                    min_counter = std::numeric_limits<DATA_TYPE>::max();
                }
                if (slots[bucket_pos][i].counters[flag] < min_counter) {
                    min_counter = slots[bucket_pos][i].counters[flag];
                    slot_pos = i;
                }
            }
            else if (!non_bursting) {
                if (slots[bucket_pos][i].counters[flag] < min_counter){
                    min_counter = slots[bucket_pos][i].counters[flag];
                    slot_pos = i;
                }
            }
        }

        if (slots[bucket_pos][slot_pos].id == 0) {
            slots[bucket_pos][slot_pos].id = id;
            slots[bucket_pos][slot_pos].window = slot.window;
            slots[bucket_pos][slot_pos].counters[flag] = count;
            slots[bucket_pos][slot_pos].counters[flag ^ 1] = slot.counters[flag ^ 1];
        }
        if( count >= slots[bucket_pos][slot_pos].counters[flag] && _searcher.find( slots[bucket_pos][slot_pos].id) == _searcher.end() )
        {  //evict
            auto oldId = slots[bucket_pos][slot_pos].id;
            auto oldCount = slots[bucket_pos][slot_pos].counters[flag];

            Slot oldSlot;
            oldSlot.id = oldId;
            oldSlot.counters[flag] = oldCount;
            oldSlot.counters[flag ^ 1] = slots[bucket_pos][slot_pos].counters[flag ^ 1];
            oldSlot.window = slots[bucket_pos][slot_pos].window;

            slots[bucket_pos][slot_pos].id = id;
            slots[bucket_pos][slot_pos].window = slot.window;
            slots[bucket_pos][slot_pos].counters[flag] = count;
            slots[bucket_pos][slot_pos].counters[flag ^ 1] = slot.counters[flag ^ 1];

            kick(oldSlot, _searcher, kickNum, index);

        }
    }

}
template<typename ID_TYPE, typename DATA_TYPE>
BTwo<ID_TYPE, DATA_TYPE>::BTwo(int b_num, int b_size, uint32_t s1_thres,uint32_t s2_thres, int _kickMaxNum) :
flag(0), bucket_num(b_num),bucket_size(b_size),stage_one_thres(s1_thres), stage_two_thres(s2_thres), kickMaxNum(_kickMaxNum){
    slots = new Slot*[bucket_num];
    for (int i = 0; i < bucket_num; ++i) {
        slots[i] = new Slot[bucket_size];
        memset(slots[i], 0, sizeof(Slot) * bucket_size);
        for (int j = 0; j < bucket_size; ++j) {
            slots[i][j].window = -1;
        }
    }
}

template<typename ID_TYPE, typename DATA_TYPE>
BTwo<ID_TYPE, DATA_TYPE>::~BTwo() {
    for (int i = 0; i < bucket_num; ++i) {
        delete [] slots[i];
    }
    delete [] slots;
}

template<typename ID_TYPE, typename DATA_TYPE>
bool BTwo<ID_TYPE, DATA_TYPE>::lookup(ID_TYPE id)  {
    for(int index = 0; index < HashNum; index++){
//        int bucket_pos = hash(id, 32 + index) % bucket_num;
        int bucket_pos = (hash(id, 32 + index) % (bucket_num / HashNum) ) + (index * (bucket_num / HashNum) );

        for (int i = 0; i < bucket_size; ++i) {
            if (slots[bucket_pos][i].id == id) {
                ++slots[bucket_pos][i].counters[flag];
                return true;
            }
        }
    }

    return false;
}


template<typename ID_TYPE, typename DATA_TYPE>
void BTwo<ID_TYPE, DATA_TYPE>::window_transition(uint32_t window, const std::multiset<uint64_t>&_searcher)  {
    // clear
    for (int i = 0; i < bucket_num; ++i) {
        for (int j = 0; j < bucket_size; ++j) {
            if (slots[i][j].id != 0 && slots[i][j].window != -1 &&
                window - slots[i][j].window > win_cold_thres ) {
                slots[i][j].window = -1;
            }
            else if (slots[i][j].id != 0 && slots[i][j].counters[0] < stage_one_thres &&
                     slots[i][j].counters[1] < stage_one_thres && _searcher.find(slots[i][j].id) == _searcher.end()) {
                memset(&slots[i][j], 0, sizeof(Slot));
                slots[i][j].window = -1;

            }
        }
    }

    // find burst
    for (int i = 0; i < bucket_num; ++i) {
        for (int j = 0; j < bucket_size; ++j) {
            if (slots[i][j].id == 0)
                continue;
            if (slots[i][j].window != -1 && slots[i][j].counters[flag] <= slots[i][j].counters[flag ^ 1] / lambda) {
                results.emplace_back(UpDownBurst<ID_TYPE>(slots[i][j].window, window, slots[i][j].id));
                slots[i][j].window = -1;
            }
            else if (slots[i][j].counters[flag] < stage_two_thres) {
                slots[i][j].window = -1;
            }
            else if (slots[i][j].counters[flag] >= lambda * slots[i][j].counters[flag ^ 1] && slots[i][j].counters[flag] >= stage_two_thres) {

                slots[i][j].window = window;
            }

        }
    }

    flag ^= 1;
    for (int i = 0; i < bucket_num; ++i)
        for (int j = 0; j < bucket_size; ++j)
            slots[i][j].counters[flag] = 0;
}


#endif //BRUSTSKETCHPERIOD_BURSTPASTSTAGETWO_H
