#ifndef _user
#define _user

#include "bpt.h"
#include "exceptions.hpp"

using user_name = str<20>;
using pass_word = str<30>
struct user_info{
    pass_word pw;
    str<20> name;
    str<30> mail;
    int pr;
    user_info():pr(-1){}
    user_info(const pass_word &_pw, const str<20> &_name, const str<30> &_mail, const int g):pw(_pw), name(_name), mail(_mail), pr(g){}
};

const int B = 100;
class user_system{
private:
    bpt<user_name,user_info,B> us;
    hash_map<25117> log;
public:
    user_system():us("user_init","user_ind","user_data"){}
    void add_user(const user_name& c, const user_name& p, const user_info& w){
        auto cur = log.find(c.hash());
        if(us.empty()) p.pr = 10;
        else{
            if(cur == log.end()) throw "no such user";
            if(log.se() <= w.pr) throw "invalid priority";
            if(us.find(p) != us.end()) throw "user exist";
        }
        us.insert(p, w);
    }

    void login(const user_name& u, const pass_word& pw){
        int id = u.hash();
        if(log.find(id) != log.end()) throw "user loggin";
        auto cur = us.find(u);
        if(cur == us.end()) throw "no such user";
        user_info v = cur.dat();
        if(v.pw != pw) throw "wrong password";
        log.insert(id, v.pr);
    }
    
    void logout(const user_name& u){
        int id = u.hash();
        if(log.find(id) == log.end()) throw "user loggout";
        log.erase(id);
    }

    bpt<user_name, user_info, B>::iterator get_profile(const user_name& c, const user_name& u, int g = -1){
        auto cur = log.find(c.hash());
        if(cur == us.end()) throw "user not loggin";
        auto now = us.find(u);
        if(now == us.end()) throw "no such user";
        user_info p = now.dat();
        if(p.pr > cur.se() || g > cur.se()) throw "invalid priorty";
        return now;
    }
};

#endif