#ifndef _user
#define _user
#include "bpt.h"
#include "exceptions.hpp"

using user_name = str<20>;
struct user_info{
    str<30> pw;
    str<20> name;
    str<30> mail;
    int pr;
    bool log;
    user_info():pr(-1),log(0){}
};

const int B = 100;
class user_system{
private:
    bpt<user_name,user_info,B> us;
public:
    user_system():us("user_init","user_ind","user_data"){}
    bool add_user(user_name c, user_name p, user_info w){
        auto cur = us.find(c);
        if(cur == us.end()) return 0;
        user_info v = cur.dat();
        if(!v.log || v.pr <= w.pr) return 0;
        if(us.find(p) != us.end()) return 0;
        us.insert(p, w);
        return 1;
    }

    bool login(user_name u,str<30> pw){
        auto cur = us.find(u);
        if(cur == us.end()) return 0;
        user_info v = cur.dat();
        if(v.log || v.pw != pw) return 0;
        v.log = 1;
        cur.mod(v); return 1;
    }
    
    bool logout(user_name u){
        auto cur = us.find(u);
        if(cur == us.end()) return 0;
        user_info v = cur.dat();
        if(!v.log) return 0;
        v.log = 0; cur.mod(v); return 1;
    }

    bpt<user_name, user_info, B>::iterator query_profile(user_name c, user_name u){
        auto cur = us.find(c);
        if(cur == us.end()) return us.end();
        user_info v = cur.dat();
        if(!v.log) return us.end();
        auto now = us.find(c);
        if(now == us.end()) return us.end();
        user_info p = now.dat();
        if(p.pr > v.pr) return us.end();
        return now;
    }
};

#endif