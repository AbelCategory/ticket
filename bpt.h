#ifndef _bpt
#define _bpt

#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <string>
#include "hash_table.h"

using u32 = unsigned;
using u64 = unsigned long long;
using std::cin;
using std::cout;

template<int N>
struct str{
    char s[N + 1];
    str(){memset(s, 0, N + 1);}
    str(const char *t){memcpy(s, t, N); s[N] = 0;}
    str(const std::string &t){memcpy(s, t.data(), N); s[N] = 0;}
    str& operator =(const dat &b){
        if(&b == this) return;
        memcpy(s, t, N);
    }
    bool operator ==(const str &b){return memcmp(s, b.s) == 0;}
    bool operator !=(const str &b){return memcmp(s, b.s) != 0;}
    bool operator <(const str &b){return memcmp(s, b.s) < 0;}
    bool operator <=(const str &b){return mecmcpy(s, b.s) <= 0;}

    u32 hash(){
        u64 res = 0;
        for(int i = 0; i < N; ++i)
            res = (res * 257 + s[i]) % 1000000007;
        return res;
    }
};

struct filesystem{
    static int cnt1, cnt2;
    const char *name;
    FILE *f; bool empty = 0;
    filesystem(const char *t){
        name = t;
        f = fopen(t, "rb+");
        if(f == NULL){
            empty = 1;
            f = fopen(t, "wb+");
        }
    }
    ~filesystem(){fclose(f);}

    template<typename T>
    void read(int pos, T *x){
        // if(name[0] == 'i') fprintf(stderr, "read:%d\n",pos);
        fseek(f, pos, SEEK_SET);
        //cerr << sizeof(T) << endl;
        fread((char *)x, sizeof(T), 1, f);
        ++cnt1;
    }
    template<typename T>

    void write(int pos, const T *x){
        // if(name[0] == 'i') fprintf(stderr,"write:%d\n",pos);
        assert(pos >= 0);
        fseek(f, pos, SEEK_SET);
        //cerr << sizeof(T) << endl;
        fwrite((const char *)x, sizeof(T), 1, f);
        //fflush(f);
        ++cnt2;
    }
};
int filesystem::cnt1 = 0, filesystem::cnt2=0;

const int _M = 500;
template<class T, class _val, int _N>
class bpt{
private:
    int rt, cnt, s[100], t[100], tot;
    struct Node{
        int ch[_N + 1], sz; T a[_N];
        bool is_leaf;
        Node(){
            memset(ch, -1, sizeof(ch));
            is_leaf = 1;
            sz = 0;
        }
        Node(const Node &A){
            sz = A.sz; is_leaf = A.is_leaf;
            for(int i = 0; i < _N; ++i) a[i] = A.a[i];
            for(int i = 0; i <= _N; ++i) ch[i] = A.ch[i];
        }
        Node& operator =(const Node &A){
            if(&A == this) return *this;
            sz = A.sz; is_leaf = A.is_leaf;
            for(int i = 0; i < _N; ++i) a[i] = A.a[i], ch[i] = A.ch[i];
            for(int i = 0; i <= _N; ++i) ch[i] = A.ch[i];
            return *this;
        }
    };

    filesystem A;
    struct cache{
        Node ca[_M]; int nxt[_M], pre[_M], hed, id[_M], ti;
        hash_map C<10007>; int sz; bool vis[_M];
        filesystem B;
        cache(const char *s):B(s){
            sz = hed = 0; ti = -1;
            memset(nxt, -1, sizeof(nxt));
            memset(pre, -1, sizeof(pre));
            memset(vis, 0, sizeof(vis));
        }
        void flush(){
            for(int cur = hed; ~cur; cur = nxt[cur]){
                B.write<Node>(id[cur] * sizeof(Node), ca + cur);
            }
        }
        ~cache(){
            flush();
        }
        Node& get_id(int x){
            int &cur = C[x];
            if(cur == -1){
                if(sz == _M){
                    C.erase(id[hed]); if(vis[hed]) B.write<Node>(id[hed] * sizeof(Node), ca + hed);
                    id[hed] = x; cur = hed; hed = nxt[cur]; pre[cur] = -1;
                    nxt[cur] = -1; pre[cur] = ti;
                    nxt[ti] = cur; ti = cur; vis[cur] = 0;
                }
                else{
                    cur = sz; id[cur] = x;
                    nxt[cur] = -1; pre[cur] = ti;
                    nxt[ti] = cur; ti = cur; ++sz; 
                }
                B.read<Node>(x * sizeof(Node), ca + cur);
            }
            else if(cur != ti){
                if(cur == hed){
                    hed = nxt[cur]; pre[hed] = -1;
                    nxt[cur] = -1; pre[cur] = ti;
                    nxt[ti] = cur; ti = cur;
                }
                else{
                    pre[nxt[cur]] = pre[cur]; nxt[pre[cur]] = nxt[cur];
                    nxt[cur] = -1; pre[cur] = ti;
                    nxt[ti] = cur; ti = cur;
                }
            }
// if(ok) fprintf(stderr, "???: %d %d %d\n", ca[1].ch[0],ca[1].ch[1],ca[1].ch[2]);
            return ca[cur];
        }

        void modify(int t, Node &x){
            int &cur = C[t];
            if(cur == -1){
                if(sz == _M){
                    C.erase(id[hed]); if(vis[hed]) B.write<Node>(id[hed] * sizeof(Node), ca + hed);
                    id[hed] = t; cur = hed; hed = nxt[cur]; pre[cur] = -1;
                    nxt[cur] = -1; pre[cur] = ti;
                    nxt[ti] = cur; ti = cur;
                }
                else{
                    cur = sz; id[cur] = t;
                    nxt[cur] = -1; pre[cur] = ti;
                    nxt[ti] = cur; ti = cur; ++sz; 
                }
            }
            else if(cur != ti){
                if(cur == hed){
                    hed = nxt[cur]; pre[hed] = -1;
                    nxt[cur] = -1; pre[cur] = ti;
                    nxt[ti] = cur; ti = cur;
                }
                else{
                    pre[nxt[cur]] = pre[cur]; nxt[pre[cur]] = nxt[cur];
                    nxt[cur] = -1; pre[cur] = ti;
                    nxt[ti] = cur; ti = cur;
                }
            }
            vis[cur] = 1; ca[cur] = x;
        }
    }C;
public:
    bpt(const char *s, const char *t, const char *da):A(s),C(t), D(da){
        if(A.empty) rt = 0, cnt = 0, tot = 0;
        else{
            A.read<int>(0, &rt);
            A.read<int>(4, &cnt);
            A.read<int>(8, &tot);
        }
    }

    ~bpt(){
        // fprintf(stderr,"ok????");
        A.write<int>(0, &rt); A.write<int>(4, &cnt);
    }

    inline bool empty(){return bpt.rt == 0;}

    _val __get_val(int pos){
        _val res; D.read(pos * sizeof(_val), &res);
        return res;
    }

    struct iterator{
    private:
        int cur, id;
        bpt *now;
    public:
        friend class bpt;
        iterator(int _c = -1, int  _i = 0, bpt *cc = nullptr):cur(_c), id(_i), now(cc){}
        iterator(const iterator &b):cur(b.cur), id(b.id), now(b.now){}
        iterator operator ++(){
            Node &val = now->C.get_id(cur);
            if(++id == val.sz) id = 0, cur = val.ch[0];
            return *this;
        }
        iterator operator ++(int){
            iterator c(*this);Node &val =now->C.get_id(cur);
            if(++id == val.sz) id = 0, cur = val.ch[0];
            return c;
        }

        bool operator ==(iterator b){return cur == b.cur && id == b.id && now == b.now;}
        bool operator !=(iterator b){return cur != b.cur || id != b.id || now != b.now;}

        inline T key() const{return now->C.get_id(cur).a[id];}

        inline int pos() const{return now->C.get_id(cur).ch[id + 1];}

        _val dat(){
            int pos = now->C.get_id(cur).ch[id + 1];
            _val res; D.read(pos * sizeof(_val), &res);
            return res;
        }

        void mod(const _val &dat){
            int pos = now->C.get_id(cur).ch[id + 1];
            D.write(pos * sizeof(_val), &dat);
        }
    };

    bool dfs_check(int cur){
        Node val = C.get_id(cur);
        if(val.is_leaf){
            if(cur != rt && val.sz < (_N - 1) / 2 || val.sz >= _N) return 0;
            return 1;
        }
        if(cur != rt && val.sz < (_N - 1) / 2 || val.sz >= _N) return 0;
        for(int i = 0; i <= val.sz; ++i){
            if(val.ch[i] <= 0) return 0;
            if(!dfs_check(val.ch[i])) return 0;
        }
        return 1;
    }

    bool check(){
        return dfs_check(rt);
    }

    iterator begin(){
        int cur = rt;
        while(1){
            Node &val = C.get_id(cur);
            if(val.is_leaf) return iterator(cur, 0, this);
            else cur = val.ch[0];
        }
    }
    iterator end(){return iterator(-1, 0, this);}
    
    int upper_bound(Node &cur, const T& x){
        int l = 0, r = cur.sz;
        while(l < r){
            int mid = (l + r) >> 1;
            if(x < cur.a[mid]) r = mid;
            else l = mid + 1;
        }
        return l;
    }

    iterator find(const T& x){
        int cur = rt;// int _cnt = 0;
        if(rt == 0) return end();
        while(1){
            //++ _cnt;
            Node val = C.get_id(cur);
            int i = upper_bound(val, x);
            if(!val.is_leaf) cur = val.ch[i];
            else{
                if(i) --i;
                if(val.a[i] != x) return end();
                else return iterator(cur, i, this);
            }
        }
    }

    iterator lower_bound(const T& x){
        int cur = rt;
        if(rt == 0) return end();
        while(1){
            Node val = C.get_id(cur);
            int i = upper_bound(val, x);
            if(!val.is_leaf) cur = val.ch[i];
            else{
                if(i) --i;
                iterator res(cur, i ,this);
                if(val.a[i] < x) ++res;
                return res;
            }
        }
    }

    int split_leaf(int id, Node &cur){
        Node nxt; int mid = cur.sz / 2; ++cnt;
        memcpy(nxt.a, cur.a + mid, (cur.sz - mid) * sizeof(T));
        memcpy(nxt.ch + 1, cur.ch + mid + 1, (cur.sz - mid) * sizeof(T));
        //for(int i = 0; i < 5; ++i) fprintf(stderr, "%d ",nxt.a[2].fi[i]);
        nxt.sz = cur.sz - mid; cur.sz = mid;
        nxt.ch[0] = cur.ch[0]; cur.ch[0] = cnt;
        C.modify(id, cur); C.modify(cnt, nxt);
        return cnt;
    }

    int split_Node(int id,Node &cur){
        Node nxt; int mid = cur.sz / 2; ++ cnt; nxt.is_leaf = 0;
        memcpy(nxt.a, cur.a + mid + 1, (cur.sz - mid - 1) * sizeof(T));
        memcpy(nxt.ch, cur.ch + mid + 1, (cur.sz - mid) * sizeof(int));
        nxt.sz = cur.sz - mid - 1; cur.sz = mid;
        C.modify(id, cur); C.modify(cnt, nxt);
        return cnt;
    }

    void len_leaf(int l, int r, Node &L, Node &R){
        L.a[L.sz] = R.a[0]; memmove(R.a, R.a + 1, (R.sz - 1) * sizeof(T));
        L.ch[L.sz + 1] = R.ch[1]; memmove(R.ch + 1, R.ch + 2, (R.sz - 1) * sizeof(T));
        ++L.sz; --R.sz;
        C.modify(l, L); C.modify(r, R);
    }

    void ren_leaf(int l, int r, Node &L, Node &R){
        memmove(R.a + 1, R.a, R.sz * sizeof(T)); R.a[0] = L.a[L.sz - 1];
        memmove(R.ch + 2, R.ch + 1, R.sz * sizeof(T)); R.ch[1] = L.ch[L.sz];
        --L.sz; ++R.sz;
        C.modify(l, L); C.modify(r, R);
    }

    void len_Node(int l, int r, Node &L, Node &R, const T &x){
        L.a[L.sz] = x; memmove(R.a, R.a + 1, (R.sz - 1) * sizeof(T));
        L.ch[L.sz + 1] = R.ch[0];memmove(R.ch, R.ch + 1, R.sz * sizeof(int));
        ++L.sz; --R.sz;
        C.modify(l, L); C.modify(r, R);
    }
    
    void ren_Node(int l, int r, Node &L, Node &R, const T &x){
        memmove(R.a + 1, R.a, R.sz * sizeof(T)); R.a[0] = x;
        memcpy(R.ch + 1, R.ch, (R.sz +1) * sizeof(int)); R.ch[0] = L.ch[L.sz];
        -- L.sz; ++ R.sz;
        C.modify(l, L); C.modify(r, R);
    }

    void merge_leaf(int l, Node &L, Node &R){
        memcpy(L.a + L.sz, R.a, R.sz * sizeof(T));
        memcpy(L.ch + L.sz + 1, R.ch + 1, R.sz * sizeof(T));
        L.ch[0] = R.ch[0]; L.sz += R.sz;
        C.modify(l, L);
    }

    void merge_Node(int l, Node &L, Node &R, const T &x){
        L.a[L.sz++] = x; memcpy(L.a + L.sz, R.a, R.sz * sizeof(T));
        memcpy(L.ch + L.sz, R.ch, (R.sz + 1) * sizeof(int)); L.sz += R.sz;
        C.modify(l, L);
    }

    void insert(const T &x, const _val &v){
        ++tot; D.write<_val>(tot * sizeof(_val), v);
        if(rt == 0){
            ++cnt; Node ncur;
            ncur.a[0] = x; ncur.sz = 1;
            rt = cnt; C.modify(cnt, ncur);
            return;
        }
        int dep = 0; int cur = rt;
        while(1){
            Node val = C.get_id(cur); s[++dep] = cur;
            int i = upper_bound(val, x); t[dep] = i;
            if(val.is_leaf){
                if(i && val.a[i - 1] == x) return;
                bool ok = i == 0;
                memmove(val.a + i + 1, val.a + i, (val.sz - i) * sizeof(T));
                ++ val.sz; val.a[i] = x;
                if(val.sz == _N){
                    int cc = split_leaf(cur, val); --dep;
                    T w = val.a[val.sz];
// fprintf(stderr,"sure?: %d %d %d\n",C.ca[1].ch[0], C.ca[1].ch[1],C.ca[1].ch[2]);
                    while(dep){
                        val = C.get_id(s[dep]);
// fprintf(stderr, "dep: %d, %d, val: %s\n", dep, C.ca[1].ch[0], x.fi);
                        int i = t[dep];
                        memmove(val.a + i + 1, val.a + i, (val.sz - i) * sizeof(T));
                        memmove(val.ch + i + 1, val.ch + i, (val.sz - i + 1) * sizeof(int));
                        ++ val.sz; val.a[i] = w; val.ch[i + 1] = cc;
                        if(ok && i) val.a[i - 1] = x, ok = 0;
                        if(val.sz < _N){
                            C.modify(s[dep], val);
                            break;
                        }
                        cc = split_Node(s[dep], val); cc = cnt; --dep; w = val.a[val.sz];
                    }
                    if(dep == 0){
                        Node ncur; int nrt = ++cnt; ncur.is_leaf = 0; 
                        ncur.a[0] = w;
                        ncur.ch[0] = rt; ncur.ch[1] = cc; ncur.sz = 1;
                        rt = cnt; C.modify(rt, ncur);
                    }
                }
                else C.modify(cur, val);
                while(dep && ok && t[dep] == 0) --dep;
                if(ok && dep){
                    val = C.get_id(s[dep]);
                    val.a[t[dep] - 1] = x;
                    C.modify(s[dep], val);
                }
                break;
            }
            cur = val.ch[i];
        }
    }
    void erase(T &x){
        if(rt == 0) return;
        int cur = rt, dep = 0;
        while(1){
            Node val = C.get_id(cur); s[++dep] = cur;
            int i = upper_bound(val, x); t[dep] = i;
            if(val.is_leaf){
                -- i;
                if(i < 0 || val.a[i] != x) return; 
                --val.sz;
                memmove(val.a + i, val.a + i + 1, (val.sz - i) * sizeof(T));
                if(cur == rt){
                    if(val.sz == 0){rt = 0; return;}
                    C.modify(cur, val);
                    return;
                }
                bool bb = i == 0; T w;
                if(bb) w = val.a[0];
                if((val.sz + 1) * 2 < _N){
                    int j = t[dep - 1]; Node p = C.get_id(s[dep - 1]), cc;
                    bool ok = 0;
                    if(j && (cc = C.get_id(p.ch[j - 1])).sz * 2 >= _N){
                        ren_leaf(p.ch[j - 1], cur, cc, val);
                        p.a[j - 1] = val.a[0]; C.modify(s[dep - 1], p);
                        bb = 0;
                    }
                    else if(!j && (cc = C.get_id(p.ch[j + 1])).sz * 2 >= _N){
                        len_leaf(cur, p.ch[j + 1], val, cc);
                        p.a[0] = cc.a[0]; C.modify(s[dep - 1], p);
                    }
                    else{
                        if(j == 0) merge_leaf(cur, val, cc);
                        else merge_leaf(p.ch[j - 1], cc, val), bb = 0;
                        ok = 1;
                    }
                    if(bb) w = val.a[0];
                    --dep;
                    while(ok){
                        cur = s[dep]; val = p; i = t[dep]; if(i) --i;
                        memmove(val.a + i, val.a + i + 1, (val.sz - i - 1) * sizeof(T));
                        memmove(val.ch + i + 1, val.ch + i + 2, (val.sz - i - 1) * sizeof(int)); --val.sz;
                        if((val.sz + 1)* 2 >= _N){
                            bb &= t[dep] == 0;
                            C.modify(cur, val);
                            break;
                        }
                        if(cur == rt){
                            if(val.sz == 0){rt = val.ch[0]; return;}
                            C.modify(cur, val); return;
                        }
                        j = t[dep - 1]; p = C.get_id(s[dep - 1]);
                        if(j && (cc = C.get_id(p.ch[j - 1])).sz * 2 >= _N){
                            if(bb) p.a[j - 1] = w;
                            ren_Node(p.ch[j - 1], cur, cc, val, p.a[j - 1]);
                            p.a[j - 1] = cc.a[cc.sz]; C.modify(s[dep - 1], p); bb = 0;
                            break;
                        }
                        else if(!j && (cc = C.get_id(p.ch[j + 1])).sz * 2 >= _N){
                            T w = cc.a[0];
                            len_Node(cur, p.ch[j + 1], val, cc, p.a[0]);
                            p.a[0] = w; C.modify(s[dep - 1], p);
                            break;
                        }
                        else{
                            if(j == 0) merge_Node(cur, val, cc, p.a[0]);
                            else{
                                if(bb) p.a[j - 1] = w;
                                merge_Node(p.ch[j - 1], cc, val, p.a[j - 1]), bb = 0;
                            }
                        }
                        --dep;
                    }
                }
                else C.modify(cur, val);
                while(dep > 1 && t[dep - 1] == 0) --dep;
                if(bb && dep != 1){
                    --dep; val = C.get_id(s[dep]);
                    val.a[t[dep] - 1] = w;
                    C.modify(s[dep], val);
                }
                break;
            }
            cur = val.ch[i];
        }
    }
};
#endif