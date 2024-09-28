#ifndef _CAIDADATASET_H_
#define _CAIDADATASET_H_

#include <cstdint>
#include <string>

struct CAIDA_Tuple {
    uint64_t timestamp;
    uint64_t id;
};

#endif