#ifndef _ticket
#define _ticket

#include "bpt.h"
#include "user.h"
#include "train.h"

struct ticket{

};

struct ticketsystem{
    user_system user;
    train_system train;
    bpt<train_sta, Tim, 100> sta;
};

#endif