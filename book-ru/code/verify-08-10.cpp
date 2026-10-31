#include "08-persistent-segment.hpp"
#include "09-persistent-trees.hpp"
#include "10-general-persistence.hpp"
#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include <functional>
#include <cassert>

int main() {
    std::mt19937 gen(20261031);
    auto rnd=[&](int n){return static_cast<int>(gen()%n);};
    for (int n=1;n<=45;++n) {
        PersistentSum t(n);
        std::vector<PersistentSum::Root> roots{0};
        std::vector<std::vector<long long>> model(1,std::vector<long long>(n));
        int bound=1; for(int w=1;w<n;w*=2) ++bound;
        for(int step=0;step<300;++step) {
            int v=rnd(static_cast<int>(roots.size())),i=rnd(n),x=rnd(101)-50;
            auto a=model[v]; a[i]=x;
            auto old=t.allocated();
            auto r=t.set(roots[v],i,x);
            assert(t.allocated()-old<=static_cast<std::size_t>(bound));
            roots.push_back(r); model.push_back(a);
            for(int k=0;k<8;++k) {
                int ver=rnd(static_cast<int>(roots.size()));
                int l=rnd(n+1),h=rnd(n+1); if(l>h) std::swap(l,h);
                old=t.allocated();
                assert(t.sum(roots[ver],l,h)==std::accumulate(
                    model[ver].begin()+l,model[ver].begin()+h,0LL));
                assert(t.allocated()==old);
            }
        }
    }
    for(int trial=0;trial<300;++trial) {
        int n=1+rnd(20); std::vector<int>a(n);
        for(int& x:a)x=rnd(11)-5;
        auto c=a; std::sort(c.begin(),c.end());
        c.erase(std::unique(c.begin(),c.end()),c.end());
        PersistentSum t(static_cast<int>(c.size()));
        std::vector<PersistentSum::Root> roots{0};
        for(int x:a) {
            int rank=static_cast<int>(std::lower_bound(c.begin(),c.end(),x)-c.begin());
            roots.push_back(t.add(roots.back(),rank,1));
        }
        for(int l=0;l<=n;++l)for(int r=l;r<=n;++r) {
            std::vector<int>b(a.begin()+l,a.begin()+r);std::sort(b.begin(),b.end());
            auto count=t.allocated();
            for(int k=1;k<=r-l;++k) assert(c[t.kth(roots[l],roots[r],k)]==b[k-1]);
            for(int x=-6;x<=6;++x) {
                int h=static_cast<int>(std::lower_bound(c.begin(),c.end(),x)-c.begin());
                auto expected=std::count_if(b.begin(),b.end(),[&](int v){return v<x;});
                assert(t.sum(roots[r],0,h)-t.sum(roots[l],0,h)==expected);
            }
            assert(t.allocated()==count);
        }
        bool bad=false;try{t.kth(roots[0],roots[n],0);}catch(const std::out_of_range&){bad=true;}
        assert(bad);
    }
    {
        using B=PersistentBST;
        std::vector<B::Root> roots{nullptr};std::vector<std::set<int>> model(1);
        for(int step=0;step<5000;++step) {
            int v=rnd(static_cast<int>(roots.size())),x=rnd(41)-20;
            auto a=model[v]; B::Root r;
            if(rnd(2)){a.insert(x);r=B::insert(roots[v],x);}
            else {a.erase(x);r=B::erase(roots[v],x);}
            if(a==model[v])assert(r==roots[v]);
            roots.push_back(r);model.push_back(a);
            for(int j=0;j<3;++j){int w=rnd(static_cast<int>(roots.size()));
                for(int key=-21;key<=21;++key)assert(B::contains(roots[w],key)==(model[w].count(key)!=0));}
        }
        B::Root chain;
        for(int x=0;x<300;++x)chain=B::insert(chain,x);
        for(int x=0;x<300;++x)assert(B::contains(chain,x));
    }
    {
        using H=PersistentLeftist;
        std::vector<H::Root> roots{nullptr};std::vector<std::multiset<long long>> model(1);
        for(int step=0;step<2000;++step) {
            int v=rnd(static_cast<int>(roots.size())),w=rnd(static_cast<int>(roots.size()));
            auto a=model[v]; H::Root r;
            int op=rnd(3);
            if(op==2 && a.size()+model[w].size()<=150) {
                a.insert(model[w].begin(),model[w].end());r=H::meld(roots[v],roots[w]);
            } else if(op==1 && !a.empty()) {a.erase(a.begin());r=H::pop(roots[v]);}
            else {int x=rnd(21)-10;a.insert(x);r=H::push(roots[v],x);}
            roots.push_back(r);model.push_back(a);
            for(int j=0;j<2;++j){int z=rnd(static_cast<int>(roots.size()));auto q=roots[z];
                assert(H::valid(q));for(long long x:model[z]){assert(H::top(q)==x);q=H::pop(q);}assert(!q);}
        }
        auto r=H::push(H::push(nullptr,5),2),q=H::meld(r,r);
        for(int x:{2,2,5,5}){assert(H::top(q)==x);q=H::pop(q);}assert(!q);
        for(int op=0;op<2;++op){bool bad=false;try{if(op)H::pop(nullptr);else H::top(nullptr);}
            catch(const std::out_of_range&){bad=true;}assert(bad);}
    }
    {
        HistoricalCell cell(4);std::vector<long long> a(1,4);int time=0;
        for(int step=0;step<1000;++step){int dt=1+rnd(5),x=rnd(101)-50;
            a.resize(time+dt+1,a.back());time+=dt;a[time]=x;cell.write(time,x);
            for(int j=0;j<10;++j){int t=rnd(time+10);assert(cell.read(t)==a[std::min(t,time)]);}}
        bool bad=false;try{cell.write(time,0);}catch(const std::invalid_argument&){bad=true;}assert(bad);
        bad=false;try{cell.read(-1);}catch(const std::out_of_range&){bad=true;}assert(bad);
    }
    for(int trial=0;trial<300;++trial){int n=1+rnd(80);
        std::vector<std::vector<int>> ch(n);std::vector<int> parent(n,-1);
        for(int i=1;i<n;++i){parent[i]=rnd(i);ch[parent[i]].push_back(i);}
        auto e=index_tree(ch);
        for(int u=0;u<n;++u)for(int v=0;v<n;++v){bool found=false;
            for(int w=v;w!=-1;w=parent[w])if(w==u)found=true;
            assert(ancestor(e,u,v)==found);}
        std::function<std::string(int)> encode=[&](int v){std::string s="(";
            for(int u:ch[v]) { s+=encode(u); }
            return s+")";};
        auto s=encode(0);auto decoded=decode(s);
        std::function<std::string(int)> again=[&](int v){std::string t="(";
            for(int u:decoded[v]) { t+=again(u); }
            return t+")";};
        assert(again(0)==s);assert(decoded.size()==ch.size());
    }
    for(std::string s:{"", ")(", "(()", "())", "()()", "(x)"}){
        bool bad=false;try{decode(s);}catch(const std::invalid_argument&){bad=true;}assert(bad);}
    std::string deep(100000,'(');deep+=std::string(100000,')');
    auto tree=decode(deep);auto e=index_tree(tree);
    assert(e.depth.back()==99999 && e.tout[0]==100000);
    std::cout<<"PASS: branching sums, prefix counts/kth, BST, leftist meld, fat cell, Euler/serialization\n";
}
