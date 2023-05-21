#ifndef _ticket
#define _ticket

#include "bpt.h"
#include "user.h"
#include "train.h"
#include "vector.hpp"
#include "map.hpp"

using ll = long long;

struct ticket{
    train_id tr;
    station s, t;
    int cost, tim, seat;
    DaTi sta, en;
    ticket(){}
    ticket(const train_id& _t, const station& S, const station& T, int c, int _s, const DaTi &fr, const DaTi &to, int ti):tr(_t), s(S), t(T), cost(c), seat(_s), sta(fr),en(to), tim(ti){}

    void out(){
        cout << tr.s << " " << s.s << " ";
        sta.out();
        cout << " -> " << t.s << " ";
        en.out();
        cout  << " " << cost << " " << seat << "\n";
    }
};

struct transfer{
    ticket A, B; int time;
    transfer():time(0){}
    transfer(const ticket& a, const ticket& b, int t):A(a),B(b),time(t){}
    inline int cost() const{return A.cost + B.cost;}
};

struct order{
    train_id id; int pos;
    station s, t;
    DaTi lea, arr;
    int statue, tic_num, cost, l, r;
    order(){statue = 0; pos = 0; l = r = 0;}
    order(const train_id& tr, const station& S, const station &T, const DaTi &_l, const DaTi &_r, int tic, int co, int L, int R):id(tr), 
    s(S), t(T),lea(_l), arr(_r), statue(0), tic_num(tic), cost(co), pos(0), l(L), r(R){}
};

const char* order_str[3] = {"[refunded]", "[pending]", "[success]"};

struct wait{
    int pos;
    int l, r, tic;
    wait(){pos = l = r = tic = 0;}
    wait(int p, int L, int R, int t):pos(p), l(L), r(R), tic(t){}
    wait(const wait &w):pos(w.pos), l(w.l), r(w.r), tic(w.tic){}
};

using wait_id = sjtu::pair<train_id, int>;

struct user_tim{
    user_name usr;
    int tim;
    user_tim(){tim = 0;}
    user_tim(user_name u, int t):usr(u), tim(t){}
    user_tim(const user_tim& b):usr(b.usr), tim(b.tim){}
    inline bool operator <(const user_tim &b) const{return usr < b.usr || usr == b.usr && tim > b.tim;}
    inline bool operator ==(const user_tim &b) const{return usr == b.usr && tim == b.tim;}
};
// using user_tim = sjtu::pair<user_name, int>;

struct ticketsystem : public user_system, public train_system{
    bpt<user_tim, order, 150> ord;
    bpt<wait_id, wait, 120> wait_list;

    ticketsystem(): ord("order_init", "order_key", "order_data"), wait_list("wait_init", "wait_key", "wait_data"){}
    //0: Time 1: Cost
    void query_ticket(const Day& d, const station& s, const station& t, bool _order){
        auto i1 = sta.lower_bound(train_sta(s, train_id()));
        auto i2 = sta.lower_bound(train_sta(t, train_id()));
        sjtu::vector<ticket> res;
        for(; i1 != sta.end() && i1.key().first == s; ++i1){
            train_id tra = i1.key().second;
            for(; i2 != sta.end() && i2.key().first == t && i2.key().second < tra; ++i2);
            if(i2 == sta.end() || i2.key().first != t) break;
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
            int zz = d.get_id() - fir_arr.d.get_id();
            DaTi st_time(Day(e.sal1) + zz, e.st_tim), en_time = st_time;
            st_time += e.lea_time[l]; en_time += e.arr_time[r];
            seat_info g = tic.find(train_ti(tra, e.sal1 + zz)).dat();
            res.push_back(ticket(tra, s, t, e.cost[r] - e.cost[l], g.qmin(l, r), st_time, en_time, e.arr_time[r] - e.lea_time[l]));
        }
        auto cmp = !_order ? [](ticket x, ticket y){
            return x.tim < y.tim || x.tim == y.tim && x.tr < y.tr;
        } : [](ticket x, ticket y){
            return x.cost < y.cost || x.cost == y.cost && x.tr < y.tr;
        };
        cout << res.size() << '\n';
        if(!res.empty()) sjtu::sort(res, 0, res.size() - 1, cmp);
        for(auto &cur:res){
            cur.out();
        }
    }

    //0: Time 1: Cost
    void query_transfer(const Day& d, const station& s, const station& t, bool _order){
        auto it = sta.lower_bound(train_sta(s, train_id()));
        auto it2 = sta.lower_bound(train_sta(t, train_id()));
        auto cmp = !_order ? [](transfer x, transfer y){
            if(x.time != y.time) return x.time < y.time;
            if(x.cost() != y.cost()) return x.cost() < y.cost();
            return x.A.tr < y.A.tr || x.A.tr == y.A.tr && x.B.tr < y.B.tr;
        } : [](transfer x, transfer y){
            if(x.cost() != y.cost()) return x.cost() < y.cost();
            if(x.time != y.time) return x.time < y.time;
            return x.A.tr < y.A.tr || x.A.tr == y.A.tr && x.B.tr < y.B.tr;
        };
        bool ok = 0; transfer ans;
        for(; it != sta.end() && it.key().first == s; ++it){
            sjtu::map<station, int> A;
            train_id tr1 = it.key().second;
            sta_info st = it.dat();
            int l = st.first, r;
            train_info tr_in = tr.__get_val(st.second);
            for(int i = st.first + 1; i < tr_in.sta_num; ++i)
                A[tr_in.st[i]] = i;
            auto j = it2;
            DaTi ar1(Day(tr_in.sal1), tr_in.st_tim), ar2(Day(tr_in.sal2), tr_in.st_tim);
            ar1 += tr_in.lea_time[l]; ar2 += tr_in.lea_time[l];
            if(d < ar1.d || ar2.d < d) continue;
            int D1 = d.get_id() - ar1.d.get_id();
            DaTi fi_S(d, ar1.t); fi_S += tr_in.lea_time[l];
            seat_info s1 = tic.find(train_ti(tr1, tr_in.sal1 + D1)).dat();
            for(; j != sta.end() && j.key().first == t; ++j){
                train_id tr2 = j.key().second;
                sta_info st1 = j.dat();
                train_info tr_out = tr.__get_val(st1.second);
                int R = st1.first;
                for(int L = R - 1; L >= 0; --L){
                    auto _it = A.find(tr_out.st[L]);
                    if(_it == A.end()) continue;
                    r = _it -> second;
                    station cur = _it -> first;
                    DaTi fi_T = fi_S; fi_T += tr_in.arr_time[r] - tr_in.lea_time[l];
                    DaTi le1(Day(tr_out.sal1), tr_out.st_tim), le2(Day(tr_out.sal2), tr_out.st_tim);
                    le1 += tr_out.lea_time[L]; le2 += tr_out.lea_time[L];
                    if(le2 < fi_T) continue;
                    DaTi se_S = le1;
                    int ptime = tr_out.sal1;
                    if(!(fi_T < le1)){
                        int D2 = fi_T.d.get_id() - le1.d.get_id();
                        se_S += D2; ptime += D2;
                        if(se_S < fi_T) se_S.d += 1, ptime++;
                    }
                    DaTi se_T = se_S;
                    se_S += tr_out.lea_time[L]; se_T += tr_out.arr_time[R];
                    seat_info s2 = tic.find(train_ti(tr2, ptime)).dat();
                    ticket A(tr1, s, cur, tr_in.cost[r] - tr_in.cost[l], s1.qmin(l, r), fi_S, fi_T, tr_in.arr_time[r] - tr_in.lea_time[l]);
                    ticket B(tr2, cur, t, tr_out.cost[R] - tr_out.cost[L], s2.qmin(L, R), se_S, se_T, tr_out.arr_time[R] - tr_out.lea_time[L]);
                    transfer now(A, B, (se_T.d.get_id() - fi_S.d.get_id()) * 1440 + (se_T.t.get_id() - fi_S.t.get_id()));
                    if(!ok) ans = now;
                    else if(cmp(now, ans)) ans = now;
                }
            }
        }
        if(!ok) cout << 0 << '\n';
        else{
            ans.A.out();
            ans.B.out();
        }
    }

    void buy_ticket(int uid, const user_name& u, const train_id& id, const station& s, const station &t, const Day& d, int num, bool q){
        auto it = tr.find(id);
        if(it == tr.end()) throw "no train";
        // auto tr_it = tic.find(train_ti(id, d.get_id()));
        // if(tr_it == tic.end()) throw "no such train on that day";
        train_info tf = it.dat(); int l = -1, r = -1;
        if(!tf.is_re) throw "train not released";
        for(int i = 0; i < tf.sta_num; ++i){
            if(s == tf.st[i]) l = i;
            else if(t == tf.st[i]) r = i;
        }
        if(l == -1 || r == -1 || l >= r) throw "no such station";
        DaTi t1(Day(tf.sal1), tf.st_tim), t2(Day(tf.sal2), tf.st_tim);
        t1 += tf.lea_time[l]; t2 += tf.lea_time[l];
        if(d < t1.d || t2.d < d) throw "no such train on that day";
        int D = d.get_id() - t1.d.get_id();
        DaTi L = t1, R = t1; L.d += D; R.d += D;
        R += tf.arr_time[r] - tf.lea_time[l];
        auto tr_it = tic.find(train_ti(id, tf.sal1 + D));
        seat_info sat = tr_it.dat();
        // DaTi L(d, tf.st_tim), R(L);
        // L += tf.lea_time[l]; R += tf.arr_time[r];
        order now(id, s, t, L, R, num, tf.cost[r] - tf.cost[l], l, r); now.pos = tr_it.pos();
        int nu = sat.qmin(l, r);
        if(nu < num){
            if(!q || q && num > tf.seat_num) throw "lack of ticket";
            else{
                now.statue = 1;
                ord.insert(user_tim(u, uid), now);
                wait_list.insert(wait_id(id, uid), wait(ord.tot, l, r, nu));
                cout << "queue\n";
            }
        }
        else{
            // if(tf.sal1 + D == 191) std::cout << " ###" << sat.seat[4];
            sat.add(l, r, num);
            // if(tf.sal1 + D == 191) std::cout << " ???" << sat.seat[4] << "!!!" << " ";
            tr_it.mod(sat); now.statue = 2;
            cout << 1ll * (tf.cost[r] - tf.cost[l]) * num << '\n';
            ord.insert(user_tim(u, uid), now);
        }
    }

    void query_order(user_name u){
        u32 id = u.hash();
        if(log.find(id) == log.end()) throw "not loggin";
        auto it = ord.lower_bound(user_tim(u, 2147483647)), cur = it;
        int cnt = 0;
        for(; it != ord.end() && it.key().usr == u; ++it) ++cnt;
        cout << cnt << '\n';
        for(auto i = cur; i != it; ++i){
            order o = i.dat();
            cout << order_str[o.statue] << " " << o.id.s << " " << o.s.s << " ";
            o.lea.out();
            cout << " -> " << o.t.s << " ";
            o.arr.out();
            cout << " " << o.cost << " " << o.tic_num << "\n";
        }
    }
    
    void refund_ticket(user_name u, int n = 1){
        u32 id = u.hash();
        if(log.find(id) == log.end()) throw "not loggin";
        auto it = ord.lower_bound(user_tim(u, 2147483647));
        for(int i = 1; i < n; ++i) ++it;
        if(it == ord.end() || it.key().usr != u) throw "not enough order";
        order o = it.dat();
        if(o.statue == 0) throw "already refunded";
        if(o.statue == 1){
            o.statue = 0; it.mod(o);
            wait_list.erase(wait_id(o.id, it.key().tim));
        }
        else{
            o.statue = 0; it.mod(o);
            auto sat = tic.__get_val(o.pos);
            sat.add(o.l, o.r, -o.tic_num);
            auto cur = wait_list.lower_bound(wait_id(o.id, 0));
            sjtu::vector<wait_id> to_erase;
            for(; cur != wait_list.end() && cur.key().first == o.id; ++cur){
                wait w = cur.dat();
                if(sat.qmin(w.l, w.r) >= w.tic){
                    sat.add(w.l, w.r, w.tic);
                    auto ord_in = ord.__get_val(w.pos);
                    ord_in.statue = 2;
                    ord.__mod_val(w.pos, ord_in);
                    to_erase.push_back(cur.key()); 
                }
            }
            tic.__mod_val(o.pos, sat);
            for(auto it = to_erase.begin(); it != to_erase.end(); ++it)
                wait_list.erase(*it);
        }
    }

    void clean(){
        us.clear();
        log.clear();
        tr.clear();
        tic.clear();
        sta.clear();
        ord.clear();
        wait_list.clear();
    }
};

#endif