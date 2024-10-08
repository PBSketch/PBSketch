#include "Comparer.h"
#include <cmath>
#include <queue>

Performance Comparer::compare(std::map<PeridocItem, PBflow> &result,
                              std::map<PeridocItem, PBflow> &ground_truth) {
    int correct = 0;
    int gtNum = 0;
    int reNum = 0;

    double aae = 0.0;
    double are = 0.0;
    for (auto &r : result) {
        reNum ++;
        if(ground_truth.find(r.first) != ground_truth.end()){
            correct++;
            auto gt = ground_truth[r.first];
            aae += abs(r.second.num_ - gt.num_);
            are += abs(r.second.num_ - gt.num_) / (double)gt.num_;
        }
    }
    for(auto &gt:ground_truth){
        gtNum ++;
    }
    aae /= correct;
    are /= correct;

    return Performance(correct, gtNum, reNum, aae, are);
}

Performance Comparer::compareTop(std::map<PeridocItem, PBflow> &result,
                                 std::map<PeridocItem, PBflow> &ground_truth, int _topK) {
    std::map<PeridocItem, PBflow> resultTop;
    std::map<PeridocItem, PBflow> gtTop;

    std::priority_queue<PBflow> rspq;
    std::priority_queue<PBflow> gtpq;

    for(auto& iter : result){
        rspq.push(iter.second);
    }
    int count = 1;
    int topkHit = 0;
    while(!rspq.empty()){
        if(topkHit > rspq.top().num_){
            break;
        }
        resultTop[rspq.top().peridocItem_] = rspq.top();
        count++;
        if(count==_topK){
            topkHit = rspq.top().num_;
        }
        rspq.pop();
    }

    for(auto& iter : ground_truth){
        gtpq.push(iter.second);
    }
    count = 1;
    topkHit = 0;
    while(!gtpq.empty()){
        if(topkHit > gtpq.top().num_){
            break;
        }
        gtTop[gtpq.top().peridocItem_] = gtpq.top();
        count++;
        if(count==_topK){
            topkHit = gtpq.top().num_;
        }
        gtpq.pop();
    }


    return compare(resultTop, gtTop);
}


