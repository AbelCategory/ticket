#ifndef _train
#define _train

#include "bpt.h"
#include "user.h"

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

struct sta{
    station name;
    
};

const int _N_ = 100;
struct train_info{
    int sta_num, seat_num;
    station st[_N_];
    int prc[_N_], tr_time[_N_], stop_time[_N_];
    Tim st_tim, ;
    char typ;
    train_info(){sta_num = 0;}

};

const int B = 100;
struct train_system{
    bpt<train_id, train_info, B> tr;
    void add_train(train_id i, const train_info &t){
        
    }
};
#endif