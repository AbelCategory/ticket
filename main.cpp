#include "user.h"
#include "train.h"
#include "ticket.h"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <string>

string buf, tim_cur, nxt_tim_cur;

ticketsystem sol;

int str_int(const string& s){
    int n = s.size(), res = 0;
    for(int i = 0; i < n; ++i)
        res = res * 10 + s[i] - 48;
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin >> tim_cur;
    while(cin >> buf){
        string a[128];
        auto reader = [&](){
            while(cin >> buf){
                if(buf[0] == '['){
                    nxt_tim_cur = buf;
                    break;
                }
                cin >> a[buf[1]];
            }
        };
        if(buf == "exit"){
            sol.exit();
            return 0;
        }
        try{
            if(buf == "add_user"){
                reader();
                user_name c(a['c']), u(a['u']);
                user_info w(a['p'], a['n'], a['m'], str_int(a['g']));
                sol.add_user(c, u, w);
                cout << 0 << endl;
            }
            if(buf == "login"){
                reader();
                user_name u(a['u']);
                pass_word pw(a['p']);
                sol.login(u, pw);
                cout << 0 << endl;
            }
            if(buf == "logout"){
                reader();
                user_name u(a['u']);
                sol.logout(u);
                cout << 0 << endl;
            }
            if(buf == "query_profile"){
                reader();
                user_name c(a['c']), u(a['u']);
                sol.query_profile();
                
            }
        }
        catch(...){
            printf("")
        }
        tim_cur = nxt_tim_cur;
    }
    return 0;
}