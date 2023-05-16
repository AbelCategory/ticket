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
    user_info():pr(-1),log(0){}
};

const int B = 100;
class user_system{
private:
    bpt<user_name,user_info,B> us;
    hash_map<25117> log;
public:
    user_system():us("user_init","user_ind","user_data"){}
    void add_user(user_name c, user_name p, user_info w){
        auto cur = log.find(c.hash());
        if(us.empty()){
            p.pr = 10;
        }
        else{
            if(cur == log.end()) throw "no such user";
            if(log.se() <= w.pr) throw "invalid priority";
            if(us.find(p) != us.end()) throw "user exist";
        }
        us.insert(p, w);
    }

    void login(user_name u,str<30> pw){
        auto cur = us.find(u);
        if(log.find(cur) != log.end())
        if(cur == us.end()) throw "login error";
        user_info v = cur.dat();
        if(v.log || v.pw != pw) throw "login error";
        v.log = 1;
    }
    
    void logout(user_name u){
        auto cur = us.find(u);
        if(cur == us.end()) return 0;
        user_info v = cur.dat();
        if(!v.log) return 0;
        v.log = 0; cur.mod(v);
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