#ifndef _ticket
#define _ticket

#include "bpt.h"
#include "user.h"
#include "train.h"
#include "vector.hpp"

struct ticket{
    train_id t;
    station s, t;
    int cost;
};



struct ticketsystem : public user_system, public train_system{
    void query_ticket(Dat d, station s, station t, bool _order){
        auto it = sta.lower_bound(sjtu::make_pair(s, train_id()));
        sjtu::vector<ticket> res;
        for(; it.key().first == t; ++it){
            train_id i = it.key().second;
            auto cur = it.dat();
            train_info train= tr.__get_val(cur.second);
            for(int i = cur.first; i < train.sta_num; ++i)
                if(train.sta[i] == t){

                }
        }
    }
    void query_transfer(){

    }
};

#endif