#ifndef _train
#define _train

#include "bpt.h"
#include "user.h"
#include "utility.hpp"

using train_id = str<20>;
using station = str<20>;

const int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int pre_mon[] = {0, 31, 59, 80, 110, 141, 171, 202, 233, 263, 294, 324, 365};
struct Day{
    int month, day;
    Day(){month = day = 1;}
    Day(int _m, int _d):month(_m), day(_d){}
    Day(const char *s){
        month = (s[0] - 48) * 10 + s[1] - 48;
        day = (s[3] - 48) * 10 + s[4] - 48;
    }
    Day(int x){
        month = lower_bound(pre_mon, pre_mon + 12, x) - pre_mon;
        day = x - mon[month];
    }
    inline bool operator <(Day a, Day b){return a.month < b.month || a.month == b.month && a.day < b.day;}
    inline int get_id(){return pre_mon[month - 1] + day;}
};

struct Tim{
    int hour, minute;
    Tim(){hour = minute = 0;}
    Tim(int _h = 0, int _m = 0):hour(_h), minute(_m){}
    Tim(const char *s){
        hour = (s[0] - 48) * 10 + s[1] - 48;
        minute = (s[3] - 48) * 10 + s[4] - 48;
    }
    Tim(int ti):hour(ti / 60), minute(ti % 60){}

    inline bool operator <(Tim a, Tim b){return a.hour < b.hour || a.}
    inline int get_id(){return hour * 60 + minute;}

    Tim operator +(int ti) const{return Tim(get_id() + ti);}
    Tim operator -(const Tim &b) const{return get_id() - b.get_id();}
    Tim operator +=(int ti){
        *this = (*this) + ti;
        return *this;
    }
};

const int _N_ = 100;
struct train_info{
    int sta_num, seat_num;
    station st[_N_];
    int prc[_N_], tr_time[_N_], stop_time[_N_];
    int sal1, sal2; Tim st_tim;
    char typ; bool is_re;
    train_info(){sta_num = 0;}
};

struct seat_info{
    int seat[_N_], sta_num, tot_seat;
    seat(int st = 0, int tot = 0):sta_num(st), tot(tot_seat){memset(seat, 0, sizeof(seat));}

    void add(int l, int r, int x){
        for(int i = l; i <= r; ++i)
            seat[i] += x;
    }
    int qmin(int l,int r){
        int res = 0
        for(int i = l; i <= r; ++i)
            res = max(res, seat[i]);
        return tot_seat - res;
    }
};

using train_ti = sjtu::pair<train_id, int>;
using train_sta = sjtu::pair<train_id, station>;

const int B = 100;
struct train_system{
    bpt<train_id, train_info, B> tr;
    bpt<traid_ti, seat_info, B> tic;
    void add_train(train_id i, const train_info &t){
        if(tr.find(i) != tr.end()) throw "train exist";
        tr.insert(i, t);
    }

    void delete_train(train_id i){
        if(tr.find(i) == tr.end()) throw "train not exist";
        tr.erase(i);
    }

    void release_train(train_id i){
        auto it = tr.find(i);
        if(it == tr.end()) throw "train not exist";
        train_info cur = it.dat();
        if(cur.is_re) throw "train has been released";
        cur.is_re = 1; it.mod(cur);
        for(int d = cur.sal1; d <= cur.sal2; ++d)
            tic.insert(sjtu::make_pair(i, d), seat(i.sta_num, seat_num));
    }

    train_info query_train(train_id i, Day d){
        auto it = tr.find(i);
        if(it == tr.end()) throw "train not exist";
        train_info cur = it.dat();
        printf("%s %c\n", i, cur.typ);
        if(d > cur.sal2 || d < cur.sal1) throw "train not exist";
        Tim ti = cur.st_tim;
        for(int i = 0; i < cur.sta_num; ++i){
            printf("%s ", cur.st[i]);
            if(i == 0) printf("xx-xx xx:xx -> ");
            else printf("%02d-%02d %02d:%02d -> ", d.month, d.day, ti.hour, ti.minute);
            if(i) ti += cur.tr[i - 1];
            if(i + 1 != cur.sta_num) printf("%02d-%02d %02d:%02d\n", d.month, d.day, ti.hour, ti.minute);
            else printf("xx-xx xx:xx\n");
        }
    }

};
#endif