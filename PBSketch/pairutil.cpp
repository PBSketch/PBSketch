//
// Created by kalen on 2024/2/17.
//
#include "pairutil.h"

bool operator == (const PTwoItemPair& a, const PTwoItemPair& b){
    return memcmp(&a, &b, sizeof(PTwoItemPair)) == 0;
}