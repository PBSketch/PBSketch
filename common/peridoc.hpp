#pragma once

#include<cstdint>
#include<vector>

class PeridocItem{
public:
    PeridocItem(int i) {
        itemKey_ = i;
        peridoc_ = i;
    }

    uint64_t itemKey_;
    uint64_t peridoc_;
    PeridocItem(){
        itemKey_ = 0;
        peridoc_ = 0;
    }
    PeridocItem(uint64_t _itemKey, uint64_t _peridoc){
        itemKey_ = _itemKey;
        peridoc_ = _peridoc;
    }
    bool operator<(const PeridocItem &p) const
    {
        return (itemKey_ < p.itemKey_) || ( (itemKey_ == p.itemKey_) && (peridoc_ < p.peridoc_)) ;
    }
    bool operator==(const PeridocItem &p) const
    {
        return itemKey_ == p.itemKey_ && peridoc_ == p.peridoc_ ;
    }
    PeridocItem& operator=(const PeridocItem &p){
        itemKey_ = p.itemKey_;
        peridoc_ = p.peridoc_;

        return *this;
    }
    PeridocItem& operator=(const int i){
        itemKey_ = i;
        peridoc_ = i;

        return *this;
    }
    uint64_t genHashSeed() const{
        return itemKey_ ^ peridoc_;
    }
};

class PBflow{
public:
    PeridocItem peridocItem_;
    int num_;
    PBflow(){
        num_ = 0;
    }
    PBflow(PeridocItem _peridocItem){
        num_ = 1;
        peridocItem_ = _peridocItem;
    }
    PBflow(PeridocItem _peridocItem, int _num){
        num_ = _num;
        peridocItem_ = _peridocItem;
    }
    PBflow operator=(const PBflow &g){
        peridocItem_ = g.peridocItem_;
        num_ = g.num_;

        return g;
    }
    bool operator<(const PBflow &g) const
    {
        return (num_ < g.num_) ;
    }
};

