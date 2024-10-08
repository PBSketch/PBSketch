#include "periodicutil.h"

bool operator == (const PeridocItemPair& a, const PeridocItemPair& b){
    return memcmp(&a, &b, sizeof(PeridocItemPair)) == 0;
}