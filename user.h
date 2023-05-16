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
    user_info():pr(-1)){}
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
        int id = u.hash();
        if(log.find(id) != log.end()) throw "user loggin";
        auto cur = us.find(u);
        if(cur == us.end()) throw "no such user";
        user_info v = cur.dat();
        if(v.pw != pw) throw "wrong password";
        log.insert(id, v.pr);
    }
    
    void logout(user_name u){
        int id = u.hash();
        if(log.find(id) == log.end()) throw "user loggout";
        log.erase(id);
    }

    bpt<user_name, user_info, B>::iterator get_profile(user_name c, user_name u){
        auto cur = log.find(c.hash());
        if(cur == us.end()) throw "user not loggin";
        auto now = us.find(u);
        if(now == us.end()) throw "no such user";
        user_info p = now.dat();
        if(p.pr > cur.se()) throw "invalid priorty";
        return now;
    }
};

#endif