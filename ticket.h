#ifndef _ticket
#define _ticket

#include "bpt.h"
#include "user.h"
#include "train.h"
#include "vector.hpp"

struct ticket{
    train_id tr;
    station s, t;
    int cost, tim, seat;
    DaTi sta, en;
    ticket(){}
    ticket(const train_id& _t, const station& S, const station& T, int c, int s, const &DaTi fr, const &DaTi to, int ti):tr(_t), s(S), t(T), cost(c), seat(s), sta(fr),en(to), tim(ti){}
};

struct transfer{
    ticket A, B;
    transfer(){}
    transfer(ticket a, ticket b):A(a),B(b){}
    inline int time(){return a.tim + b.tim;}
    inline 
}

struct ticketsystem : public user_system, public train_system{
    ticketsystem(){}
    void query_ticket(const Dat& d, const station& s, const station& t, bool _order){
        auto it = sta.lower_bound(train_sta(s, train_id()));
        sjtu::vector<ticket> res;
        for(; it.key().first == t; ++it){
            int x = it.key().first;
            train_id id = it.key().second;
            auto cur = it.dat();
            train_info train= tr.__get_val(cur.second);
            auto p = tic.lower_bound();
            seat_info sat = ;
            DaTi fr(d, cur.st_tim), ar = fr; fr += cur.lea_time[x];
            for(int i = x; i < train.sta_num; ++i)
                if(train.sta[i] == t){
                    ar += cur.arr_time[i];
                    res.push_back(ticket(id, s, t, cost[i] - cost[x], ));
                    break;
                }
        }
        sort(res.begin(), res.end(), );
    }

    void query_transfer(const Dat& d, const station& s, const station& t, bool _order){
        auto it = sta.lower_bound(train_sta(s, train_id()));
        hash_map h;
    }
    
    void exit(){
        
    }
};

#endif