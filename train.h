#ifndef _train
#define _train

#include "bpt.h"
#include "user.h"

using train_id = str<20>;
using station = str<20>;

struct sta{
    station name;
    
};
struct train_info{
    int sta_num;
    station st[100];

};
#endif