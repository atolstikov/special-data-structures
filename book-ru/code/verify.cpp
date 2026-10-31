#include "01-min-queue.hpp"
#include "02-segment-tree.hpp"
#include "03-lazy-tree.hpp"
#include "04-bitset.hpp"
#include <iostream>
#include <deque>
#include <numeric>
#include <random>
#include <set>
#include <queue>

struct Summary { long long sum, pref, suff, best; };
Summary leaf(long long x) { long long p=std::max(0LL,x); return {x,p,p,p}; }
Summary merge(Summary l,Summary r) {
    return {l.sum+r.sum,std::max(l.pref,l.sum+r.pref),
            std::max(r.suff,r.sum+l.suff),
            std::max({l.best,r.best,l.suff+r.pref})};
}
int main() {
    std::mt19937 rng(20261031);
    MinQueue q; std::deque<long long> ref;
    for(int step=0;step<20000;++step) {
        if(ref.empty() || rng()%3) {long long x=int(rng()%101)-50;q.push(x);ref.push_back(x);}
        else {q.pop();ref.pop_front();}
        assert(q.empty()==ref.empty());
        if(!ref.empty()) {assert(q.front()==ref.front());assert(q.minimum()==*std::min_element(ref.begin(),ref.end()));}
    }
    MinQueue empty;
    int caught=0;
    try{empty.front();}catch(const std::out_of_range&){++caught;}
    try{empty.minimum();}catch(const std::out_of_range&){++caught;}
    try{empty.pop();}catch(const std::out_of_range&){++caught;}
    assert(caught==3);
    for(int n=0;n<=40;++n) {
        std::vector<long long>a(n,0),point(n,0);
        SumTree st(point);LazySumTree lazy(a);
        for(int k=0;k<1200;++k){
            int l=rng()%(n+1),r=rng()%(n+1);if(l>r)std::swap(l,r);
            long long x=int(rng()%101)-50;
            if(rng()%2){lazy.set(l,r,x);for(int i=l;i<r;++i)a[i]=x;}
            else {lazy.add(l,r,x);for(int i=l;i<r;++i)a[i]+=x;}
            if(n){int pos=rng()%n;st.set(pos,x);point[pos]=x;}
            for(int z=0;z<5;++z){int u=rng()%(n+1),v=rng()%(n+1);if(u>v)std::swap(u,v);
                assert(lazy.sum(u,v)==std::accumulate(a.begin()+u,a.begin()+v,0LL));
                assert(st.sum(u,v)==std::accumulate(point.begin()+u,point.begin()+v,0LL));}
        }
    }
    for(int it=0;it<3000;++it){
        int n=rng()%15;std::vector<long long>a(n);Summary s{0,0,0,0};
        for(auto&x:a){x=int(rng()%21)-10;s=merge(s,leaf(x));}
        long long best=0;for(int l=0;l<n;++l){long long sum=0;for(int r=l;r<n;++r){sum+=a[r];best=std::max(best,sum);}}
        assert(s.best==best);
        int cut=rng()%(n+1);Summary l{0,0,0,0},r{0,0,0,0};
        for(int i=0;i<cut;++i)l=merge(l,leaf(a[i]));
        for(int i=cut;i<n;++i)r=merge(r,leaf(a[i]));
        auto joined=merge(l,r);assert(joined.sum==s.sum&&joined.pref==s.pref&&joined.suff==s.suff&&joined.best==s.best);
    }
    for(int n: {0,1,2,63,64,65,127,129,257}){
        for(int it=0;it<100;++it){Bits a(n),b(n);std::vector<int>x(n),y(n);
            for(int i=0;i<n;++i){if(rng()%2){a.set(i);x[i]=1;}if(rng()%2){b.set(i);y[i]=1;}}
            if(n){int i=rng()%n;a.set(i);a.set(i);x[i]=1;i=rng()%n;a.reset(i);x[i]=0;}
            assert(a.count()==std::accumulate(x.begin(),x.end(),0));
            Bits u=a,v=a,w=a,z=a;u.unite(b);v.intersect(b);w.subtract(b);z.xor_with(b);
            for(int i=0;i<n;++i){assert(u.test(i)==bool(x[i]|y[i]));assert(v.test(i)==bool(x[i]&y[i]));assert(w.test(i)==bool(x[i]&&!y[i]));assert(z.test(i)==bool(x[i]^y[i]));}
            for(int d:{0,1,63,64,65,n,n+1}){Bits s=a.shifted(d);int count=0;for(int i=0;i<n;++i){bool expected=i>=d&&x[i-d];assert(s.test(i)==expected);count+=expected;}assert(s.count()==count);}
        }
    }
    for(int it=0;it<400;++it){
        int n=1+rng()%18;std::vector<Bits>a(n,Bits(n));std::vector<std::vector<int>>adj(n);
        for(int i=0;i<n;++i)for(int j=0;j<n;++j)if(rng()%5==0){a[i].set(j);adj[i].push_back(j);}
        auto reach=closure(a);
        for(int s=0;s<n;++s){std::vector<int>d(n,-1);std::queue<int>q;q.push(s);d[s]=0;
            while(!q.empty()){int v=q.front();q.pop();for(int u:adj[v])if(d[u]<0){d[u]=d[v]+1;q.push(u);}}
            assert(bit_bfs(a,s)==d);for(int j=0;j<n;++j)assert(reach[s].test(j)==(d[j]>=0));
        }
    }
    for(int it=0;it<1000;++it){int m=rng()%8,n=rng()%8;std::vector<Bits>a(m,Bits(n));std::vector<unsigned>raw(m);
        for(int i=0;i<m;++i)for(int j=0;j<n;++j)if(rng()%2){a[i].set(j);raw[i]|=1u<<j;}
        std::set<unsigned>span;
        for(int mask=0;mask<(1<<m);++mask){unsigned x=0;for(int i=0;i<m;++i)if(mask>>i&1)x^=raw[i];span.insert(x);}
        assert(span.size()==(1u<<gf2_rank(a,n)));
    }
    for(int it=0;it<1000;++it){int n=rng()%12,W=130;std::vector<int>a(n);Bits reachable(W+1);reachable.set(0);
        for(auto&x:a){x=rng()%80;auto shifted=reachable.shifted(x);reachable.unite(shifted);}
        std::vector<int>ref(W+1);for(int mask=0;mask<(1<<n);++mask){int sum=0;for(int i=0;i<n;++i)if(mask>>i&1)sum+=a[i];if(sum<=W)ref[sum]=1;}
        for(int s=0;s<=W;++s)assert(reachable.test(s)==bool(ref[s]));
    }
    std::cout<<"PASS: queue, sum tree, lazy set/add, max subarray, bitset boundaries, BFS, closure, GF(2), subset sums\n";
}
