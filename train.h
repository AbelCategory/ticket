#ifndef _train
#define _train

#include "bpt.h"
#include "user.h"
#include "utility.hpp"

using train_id = str<20>;
using station = str<30>;

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
        month = std::lower_bound(pre_mon, pre_mon + 12, x) - pre_mon;
        day = x - pre_mon[month - 1];
    }
    Day(const Day &b):month(b.month), day(b.day){}
    void out(){cout << to_str2(month) << "-" << to_str2(day);}
    inline int get_id() const{return pre_mon[month - 1] + day;}

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
        while(day > mon[month]) day -= mon[month], ++month;
        return *this;
    }
};

struct Tim{
    int hour, minute;
    Tim(){hour = minute = 0;}
    Tim(int _h, int _m):hour(_h), minute(_m){}
    Tim(const char *s){
        hour = (s[0] - 48) * 10 + s[1] - 48;
        minute = (s[3] - 48) * 10 + s[4] - 48;
    }
    Tim(int ti):hour(ti / 60), minute(ti % 60){}
    void out(){cout <<  to_str2(hour) << ":" << to_str2(minute);}

    inline int get_id() const{return hour * 60 + minute;}

    inline bool operator <(const Tim& b) const{return hour < b.hour || hour == b.hour && minute < b.minute;}
    inline bool operator <=(const Tim& b) const{return hour < b.hour || hour == b.hour && minute <= b.minute;}
    inline bool operator ==(const Tim& b) const{return hour == b.hour && minute == b.minute;}
    inline bool operator !=(const Tim &b) const{return hour != b.hour || minute != b.minute;}

    inline Tim operator +(int ti) const{return Tim(get_id() + ti);}
    inline Tim operator -(int ti) const{return Tim(get_id() - ti);}
    inline Tim operator -(const Tim &b) const{return get_id() - b.get_id();}
    inline Tim operator +=(int ti){
        *this = (*this) + ti;
        return *this;
    }
    inline Tim operator -=(int ti){
        *this = (*this) - ti;
        return *this;
    }
};

struct DaTi{
    Day d; Tim t;
    DaTi(){}
    DaTi(Day _d, Tim _t):d(_d), t(_t){}
    DaTi(const DaTi& b):d(b.d), t(b.t){}
    void out(){d.out(); cout << " "; t.out();}

    inline bool operator <(const DaTi& b) const{return d < b.d || d == b.d && t < b.t;}
    inline bool operator <=(const DaTi& b) const{return d < b.d || d == b.d && t <= b.t;}
    inline bool operator ==(const DaTi& b) const{return d == b.d && t == b.t;}
    inline bool operator !=(const DaTi &b) const{return d != b.d || t != b.t;}
    
    inline DaTi operator +=(int b){
        t += b;
        d += t.hour / 24;
        t.hour %= 24;
        return *this;
    }
};

const int _N_ = 100;
struct train_info{
    int sta_num, seat_num;
    station st[_N_];
    int cost[_N_], arr_time[_N_], lea_time[_N_];
    int sal1, sal2; Tim st_tim;
    char typ; bool is_re;
    train_info(){
        sta_num = 0; memset(cost, 0, sizeof(cost));
        memset(arr_time, 0, sizeof(arr_time)); memset(lea_time, 0, sizeof(lea_time));
        sal1 = sal2 = 0; is_re = 0;
    }
    train_info(int _st, int sat, int s1, int s2, const Tim& ti, char &ch):sta_num(_st), seat_num(sat), sal1(s1), sal2(s2), st_tim(ti), typ(ch){
        memset(cost, 0, sizeof(cost));
        memset(arr_time, 0, sizeof(arr_time));
        memset(lea_time, 0, sizeof(lea_time));
        is_re = 0;
    }
};

struct seat_info{
    int seat[_N_], sta_num, tot_seat;
    seat_info(int st = 0, int tot = 0):sta_num(st), tot_seat(tot){memset(seat, 0, sizeof(seat));}

    void add(int l, int r, int x){
        for(int i = l; i < r; ++i)
            seat[i] += x;
    }
    int qmin(int l,int r){
        int res = 0;
        for(int i = l; i < r; ++i)
            res = std::max(res, seat[i]);
        return tot_seat - res;
    }
};

using train_ti = sjtu::pair<train_id, int>;
using train_sta = sjtu::pair<station, train_id>;

// station_pos file_pos
using sta_info = sjtu::pair<int, int>;

struct train_system{
    bpt<train_id, train_info, 100> tr;
    bpt<train_ti, seat_info, 100> tic;
    bpt<train_sta, sta_info, 100> sta;

    train_system():tr("train_init", "train_key", "train_data"), tic("tic_init", "tic_key", "tic_data"), sta("station_init", "station_key", "station_data"){}
    
    void add_train(const train_id& id, const train_info &t){
        if(tr.find(id) != tr.end()) throw "train exist";
        tr.insert(id, t);
    }

    void delete_train(train_id id){
        auto it = tr.find(id);
        if(tr.find(id) == tr.end()) throw "train not exist";
        train_info cur = it.dat();
        if(cur.is_re) throw "cannot delete";
        tr.erase(id);
    }

    void release_train(train_id id){
        auto it = tr.find(id);
        if(it == tr.end()) throw "train not exist";
        train_info cur = it.dat();
        if(cur.is_re) throw "train has been released";
        cur.is_re = 1; it.mod(cur);
        for(int d = cur.sal1; d <= cur.sal2; ++d)
            tic.insert(train_ti(id, d), seat_info(cur.sta_num, cur.seat_num));
        for(int j = 0; j < cur.sta_num; ++j)
            sta.insert(train_sta(cur.st[j], id), sta_info(j, it.pos()));
    }

    void query_train(train_id i, Day d){
        auto it = tr.find(i);
        if(it == tr.end()) throw "train not exist";
        train_info cur = it.dat();
        if(d.get_id() < cur.sal1 || d.get_id() > cur.sal2) throw "train not exist";
        cout << i.s << " " << cur.typ << '\n';
        DaTi res(d, cur.st_tim);
        seat_info sat;
        if(cur.is_re){
            auto j = tic.find(train_ti(i, d.get_id()));
            sat = seat_info(cur.sta_num, cur.seat_num);
        }
        else sat = seat_info(cur.sta_num, cur.seat_num);
        for(int i = 0; i < cur.sta_num; ++i){
            cout << cur.st[i].s << ' ';
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