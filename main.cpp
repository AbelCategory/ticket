#include "user.h"
#include "train.h"
#include "ticket.h"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <string>

std::string buf, tim_cur, nxt_tim_cur;

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
        std::string a[128];
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
                cout << 0 << '\n';
            }
            if(buf == "login"){
                reader();
                user_name u(a['u']);
                pass_word pw(a['p']);
                sol.login(u, pw);
                cout << 0 << '\n';
            }
            if(buf == "logout"){
                reader();
                user_name u(a['u']);
                sol.logout(u);
                cout << 0 << '\n';
            }
            if(buf == "query_profile"){
                reader();
                user_name c(a['c']), u(a['u']);
                auto it = sol.query_profile();
                user_info w = it.dat();
                cout << c.s << " " << w.name.s << " " << w.mail.s << " " << w.pr << '\n';
            }
            if(buf == "modify_profile"){
                reader();
                user_name c(a['c']), u(a['u']);
                int g = -1;
                if(a['g'].size()) g = str_int(a['g']);
                auto it = sol.query_profile();
                user_info w = it.dat();
                if(g != -1) w.pr = g;
                if(a['n'].size()) w.name = str<20>(a['n']);
                if(a['m'].size()) w.mail = str<30>(a['m']);
                if(a['p'].size()) w.pw = pass_word(a['p']);
                it.mod(w);
            }
            if(buf == "add_train"){
                reader();
                train_id id(a['i']);
                std::string S = a['d'].substr(0, 5), T = a['d'].substr(6, 5);
                Dat st(S.data()), en(T.data());
                train_info info(a['n'], a['m'], d, st.get_id(), en.get_id(), Tim(a['x']), a['y'][0]);
                int p = 0, x = 0, len = a['s'].size();
                for(int i = 0; i < len; ++i){
                    if(a['s'][i] == '|') ++p, x = 0;
                    else info.st[p].s[x++] = a['s'][i];
                }
                p = 1, x = 0, len = a['p'].size();
                for(int i = 0; i < len; ++i){
                    if(a['p'][i] == '|') ++p;
                    else info.cost[p] = info.cost[p] * 10 + a['p'][i] - 48;
                }
                for(int i = 1; i < info.sta_num; ++i)
                    info.cost[i] += info.cost[i - 1];
                p = 1, x = 0, len = a['t'].size();
                for(int i = 0; i < len; ++i){
                    if(a['t'][i] == '|') ++p;
                    else info.arr_time[p] = info.arr_time[p] * 10 + a['t'][i] - 48;
                }
                p = 1, x = 0, len = a['o'].size();
                for(int i = 0; i < len; ++i){
                    if(a['o'][i] == '|') ++p;
                    else info.lea_time[p] = info.lea_time[p] * 10 + a['o'][i] - 48;
                }
                for(int i = 1; i < info.sta_num; ++i){
                    info.arr_time[i] += info.lea_time[i - 1];
                    info.lea_time[i] += info.arr_time[i];
                }

            }
            if(buf == "delete_train"){
                reader();
                train_id id(a['i']);
                ticketsystem.delete_train(id);
                cout << 0 << '\n';
            }
            if(buf == "release_train"){
                reader();
                train_id id(a['i']);
                ticketsystem.release_train(id);
                cout << 0 << '\n';
            }
            if(buf == "query_train"){
                reader();
                train_id id(a['i']);
                Dat d(a['d'].data());
                ticketsystem.query_train(id, d);
            }
            if(buf == "query_ticket"){
                reader();
                Dat d(a['d'].data());
                Tim s(a['s'].data()), t(a['t'].data());
                ticketsystem.query_ticket(d, s, t, );   
            }
            if(buf == "query_transfer"){
                reader();
                Dat d(a['d'].data());
                Tim s(a['s'].data()), t(a['t'].data());
                ticketsystem.query_transfer(d, s, t);
            }
            if(buf == "buy_ticket"){
                reader();
                user_name u(a['u']);
                train_id id(a['i']);
                Dat d(a['d']); station f(a['f']), t(a['t']);
                int num = str_int(a['n']);
                
            }
            if(buf == "query_order"){

            }
            if(buf == "refund_ticket"){

            }
        }
        catch(...){
            cout << -1 << '\n';
        }
        tim_cur = nxt_tim_cur;
    }
    return 0;
}