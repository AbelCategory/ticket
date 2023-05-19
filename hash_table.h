#ifndef _hash_tb
#define hash_tb

using u32 = unsigned;
template<int N>
struct hash_map{
    u32 a[N];
    int nxt[N], val[N], hed[N];
    int st[N], tp;
    hash_map(){
        memset(a, -1, sizeof(a));
        memset(nxt, -1 ,sizeof(nxt));
        memset(hed, -1, sizeof(hed));
        for(int i = 0; i < N; ++ i)
            st[i] = i;
        tp = 0;
    }

    void clear(){
        memset(a, -1, sizeof(a));
        memset(nxt, -1 ,sizeof(nxt));
        memset(hed, -1, sizeof(hed));
        for(int i = 0; i < N; ++ i)
            st[i] = i;
        tp = 0;
    }

    struct iterator{
        int pos; hash_map *cur;
        iterator(int _pos = -1, hash_map* _c = nullptr):pos(_pos), cur(_c){}
        iterator(const iterator& b):pos(b.pos), cur(b.cur){}
        iterator& operator =(const iterator &b){
            if(&b == this) return;
            pos = b.pos; cur = b.cur;
            return *this;
        }

        u32 fi() const{return val[pos];}
        int& se() const{return a[pos];}

        inline bool operator ==(const iterator &b){return pos == b.pos && cur == b.cur;}
        inline bool operator !=(const iterator &b){return pos != b.pos || cur != b.cur;}
    };

    iterator end() const{return iterator(-1, this);}

    iterator find(u32 x){
        int y = x % N;
        for(int i = hed[y]; ~i; i = nxt[i])
            if(val[i] == x) return iterator(i, this);
        return end();
    }

    void insert(u32 x, int t){
        int y = x % N;
        for(int i = hed[y]; ~i; i = nxt[i])
            if(val[i] == x) return;
        int cur = st[tp++];
        nxt[cur] = hed[y]; val[cur] = x;
        hed[y] = cur; a[cur] = t;
    }

    int &operator [](u32 x){
        int y = x % N;
        for(int i = hed[y]; ~i; i = nxt[i])
            if(val[i] == x) return a[i];
        int cur = st[tp++];
        nxt[cur] = hed[y]; val[cur] = x;
        hed[y] = cur; a[cur] = -1;
        return a[cur];
    }

    void erase(u32 x){
        int y = x % N;
        for(int i = hed[y], pre = -1; ~i; i = nxt[pre = i])
            if(val[i] == x){
                if(pre == -1){
                    hed[y] = nxt[i];
                    st[--tp] = i;
                }
                else{
                    nxt[pre] = nxt[i];
                    st[--tp] = i;
                }
            }
    }
};
#endif