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
    ticket(const train_id& _t, const station& S, const station& T, int c, int _s, const DaTi &fr, const DaTi &to, int ti):tr(_t), s(S), t(T), cost(c), seat(_s), sta(fr),en(to), tim(ti){}
};

struct transfer{
    ticket A, B; int time;
    transfer():time(0){}
    transfer(const ticket& a, const ticket& b, int t):A(a),B(b),time(t){}
    inline int cost() const{return A.cost + B.cost;}
};

struct order{
    train_id id;
    station s, t;
    DaTi lea, arr;
    int statue;
    order(){}
};

struct wait{

};

using wait_id = sjtu::pair<train_id, int>;
using user_tim = sjtu::pair<user_name, int>;

struct ticketsystem : public user_system, public train_system{
    bpt<user_tim, order, 150> ord;
    bpt<wait_id, wait, 150> wait_list;
    ticketsystem(): ord("order_init", "order_key", "order_data"), wait_list("wait_init", "wait_key", "wait_data"){}
    //0: Time 1: Cost
    void query_ticket(const Day& d, const station& s, const station& t, bool _order){
        auto i1 = sta.lower_bound(train_sta(s, train_id()));
        auto i2 = sta.lower_bound(train_sta(t, train_id()));
        sjtu::vector<ticket> res;
        for(; i1.key().first == s; ++i1){
            train_id tra = i1.key().second;
            for(; i2.key().first == t && i2.key().second < tra; ++i2);
            if(i2.key().first != t) break;
            if(i2.key().second != tra) continue;
            sta_info S = i1.dat(), T = i2.dat();
            int l = S.first, r = T.first;
            if(l > r) continue;
            int x = S.second;
            train_info e = tr.__get_val(x);
            if(!e.is_re) continue;
            DaTi fir_arr(Day(e.sal1), e.st_tim), lst_arr(Day(e.sal2), e.st_tim);
            fir_arr += e.lea_time[l]; lst_arr += e.lea_time[l];
            if(d < fir_arr.d|| lst_arr.d < d) continue;
            int x = d.get_id() - fir_arr.d.get_id();
            
            seat_info g = tic.find(train_ti(tra, d.get_id())).dat();
            res.push_back(ticket(tra, s, t, e.cost[r] - e.cost[l], g.qmin(l, r), , , e.arr[r] - e.lea[l]));
        }
        auto cmp = _order ? [](ticket x, ticket y){
            return x.tim < y.tim || x.tim == y.tim && x.tr < y.tr;
        } : [](ticket x, ticket y){
            return x.cost < y.cost || x.cost == y.cost && x.tr < y.tr;
        };
        cout << res.size() << '\n';
        if(!res.empty()) sjtu::sort(res, 0, res.size() - 1, );
        for(auto &cur:res){
            cout << cur.tr.s << " " << cur.s.s << " ";
            cur.sta.out();
            cout << " -> ";
            cur.en.out();
            cout << " " << cur.cost << " " << cur.seat << "\n";
        }
    }

    //0: Time 1: Cost
    void query_transfer(const Day& d, const station& s, const station& t, bool _order){
        auto it = sta.lower_bound(train_sta(s, train_id()));
        auto Min = _order ? [](transfer x, transfer y){
            if(x.time != y.time) return x.time < y.time;
            if(x.cost() != y.cost()) return x.cost() < y.cost();
            return x.A.tr < y.A.tr || x.A.tr == y.A.tr && x.B.tr < y.B.tr;
        } : [](transfer x, transfer y){
            if(x.cost() != y.cost()) return x.cost() < y.cost();
            if(x.time != y.time) return x.time < y.time;
            return x.A.tr < y.A.tr || x.A.tr == y.A.tr && x.B.tr < y.B.tr;
        };
        hash_map h;
    }

    void buy_ticket(int uid, const user_name& u, const train_id& id, const station& s, const station &t, const Dat& d, int num, bool q){
        auto it = tr.find(id);
        auto tr_it = tic.find(train_ti(id, d.get_id()));
        if(tr_it == tic.end()) throw "no such train on that day";
        train_info tf = it.dat(); int l = -1, r = -1;
        for(int i = 0; i < tf.sta_num; ++i){
            if(s == tf.st[i]) l = i;
            else if(s == tf.st[i]) r = i;
        }
        if(l == -1 || r == -1 || l < r) throw "no such station";
        seat_info sat = tr_it.dat();
        order now();
        int nu = sat.qmin(l, r);
        if(nu < num){
            if(!q){
                ord.insert(user_tim(u, uid), now);
                throw "lack of ticket";
            }
            else{
                wait_list.insert();
                cout << "queue\n";
                now.statue = 1;
            }
        }
        else{
            sat.add(l, r, num);
            tr_it.mod(sat); now.statue = 2;
            cout << 1ll * (tf.cost[r] - tf.cost[l]) * num << '\n';
        }
        ord.insert(user_tim(u, uid), now);
    }
    
    void refund_ticket(){

    }
};

#endif