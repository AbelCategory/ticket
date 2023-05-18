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
    Day(const Day &b):month(b.month), day(b.day){}
    void out(){cout << month << "-" << day;}
    inline int get_id(){return pre_mon[month - 1] + day;}

    inline bool operator <(const Day& b) const{return month < b.month || month == b.month && day < b.day;}
    inline bool operator <=(const Day& b) const{return month < b.month || month == b.month && day <= b.day;}
    inline bool operator ==(const Day &b) const{return month == b.month && day == b.day;}   
    inline bool operator !=(const Day &b) const{return month != b.month || day != b.day;}

    inline Day operator +(int b) const{
        Day res(*this);
        res.day += b;
        while(res.day > mon[res.month]) res.day -= mon[res.month], ++res.month;
        return res;
    }

    inline Day operator +=(int b){
        day += b;
        while(day > mon[month]) day -= mon[month], ++res;
        return *this;
    }
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
    void out(){cout <<  hour << ":" << minute;}

    inline int get_id(){return hour * 60 + minute;}

    inline bool operator <(const Tim& b) const{return hour < b.hour || hour == b.hour && minute < b.minute;}
    inline bool operator <=(const Tim& b) const{return hour < b.hour || hour == b.hour && minute <= b.minute;}
    inline bool operator ==(const Tim& b) const{return hour == b.hour && minute == b.minute;}
    inline bool operator !=(const Tim &b) const{return hour != b.hour || minute != b.minute;}

    inline Tim operator +(int ti) const{return Tim(get_id() + ti);}
    inline Tim operator -(const Tim &b) const{return get_id() - b.get_id();}
    inline Tim operator +=(int ti){
        *this = (*this) + ti;
        return *this;
    }
};

struct DaTi{
    Day d; Tim t;
    DaTi(){}
    DaTi(Day _d, Tim _t):d(_d), t(_t){}
    DaTi(const DaTi& b):d(b.d), t(b.t){}
    void out(){d.out(); putchar(' '); t.out();}

    inline bool operator <(const DaTi& b) const{return d < b.d || d == b.d && t < b.t;}
    inline bool operator <=(const DaTi& b) const{return d < b.d || d == b.d && t <= b.t;}
    inline bool operator ==(const DaTi& b) const{return d == b.d && t == b.t;}
    inline bool operator !=(const DaTi &b) const{return d != b.d || t != b.t;}
    
    inline DaTi operator +=(int b){
        t += b;
        d += t.hour / 24;
        t.hour %= 24;
        return 
    }
}

const int _N_ = 100;
struct train_info{
    int sta_num, seat_num;
    station st[_N_];
    int cost[_N_], tr_time[_N_], stop_time[_N_];
    int arr_time[_N_], lea_time[_N_];
    int sal1, sal2; Tim st_tim;
    char typ; bool is_re;
    train_info(){sta_num = 0;}
};

struct seat_info{
    int seat[_N_], sta_num, tot_seat;
    seat(int st = 0, int tot = 0):sta_num(st), tot(tot_seat){memset(seat, 0, sizeof(seat));}

    void add(int l, int r, int x){
        for(int i = l; i < r; ++i)
            seat[i] += x;
    }
    int qmin(int l,int r){
        int res = 0
        for(int i = l; i < r; ++i)
            res = max(res, seat[i]);
        return tot_seat - res;
    }
};

using train_ti = sjtu::pair<train_id, int>;
using train_sta = sjtu::pair<station, train_id>;
using sta_info = sjtu::pair<int, int>;

const int B = 100;
struct train_system{
    bpt<train_id, train_info, B> tr;
    bpt<traid_ti, seat_info, B> tic;
    bpt<train_sta, sta_info, 100> sta;
    train_system():tr("train_init", "train_key", "train_data"), tic("tic_init", "tic_key", "tic_data"), sta("station_init", "station_key", "station_init"){}
    void add_train(const train_id& i, const train_info &t){
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
            tic.insert(train_ti(i, d), seat_info(cur.sta_num, cur.seat_num));
        for(int j = 0; j < sta_num; ++j)
            sta.insert(train_sta(st[j], i), sta_info(j, it.pos()));
    }

    train_info query_train(train_id i, Day d){
        auto it = tr.find(i);
        if(it == tr.end()) throw "train not exist";
        train_info cur = it.dat();
        if(d.get_id() < sal1 || d.get_id() > sal2) throw "train not exist";
        cout << i.s << " " << cur.typ << '\n';
        printf("%s %c\n", i, cur.typ);
        DaTi res(d, cur.st_tim);
        seat_info sat;
        if(i.is_re){
            auto j = tic.find(train_ti(i, d));
            sat = seat_info(cur.sta_num, cur.seat_num);
        }
        else sat = seat_info(i.sta_num, seat_num);
        for(int i = 0; i < cur.sta_num; ++i){
            std::cout << cur.st[i] << ' ';
            //printf("%s ", cur.st[i]);
            if(i == 0) cout << "xx-xx xx:xx -> ";
            else res.out(), cout << " -> ";
            if(i) res += cur.lea_time[i] - cur.arr_time[i];
            if(i + 1 != cur.sta_num) res.out();
            else cout << "xx-xx xx:xx";
            if(i + 1 != cur.sta_num) cout << ' ' << cur.cost[i] << ' ' << cur.seat_num - sat.seat[i] << '\n';
            else cout << " " << cur.cost[i] << " x\n";
            if(i + 1 != cur.sta_num) res += cur.arr_time[i + 1] - cur.lea_time[i];
        }
    }

};
#endif