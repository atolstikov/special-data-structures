#include "05-decomposition.hpp"
#include "06-mitm.hpp"
#include "07-persistence.hpp"
#include <iostream>
#include <numeric>
#include <random>
#include <deque>

int main() {
    std::mt19937 rng(20261101);
    for (int n=0; n<=60; ++n) for (int b:{1,2,3,7,11,64}) {
        std::vector<long long> a(n);
        for (auto& x:a) x=int(rng()%41)-20;
        BlockSum blocks(a,b);
        for (int k=0;k<200;++k) {
            if (n) {int i=rng()%n; long long x=int(rng()%41)-20; blocks.set(i,x);a[i]=x;}
            int l=rng()%(n+1),r=rng()%(n+1);if(l>r)std::swap(l,r);
            assert(blocks.sum(l,r)==std::accumulate(a.begin()+l,a.begin()+r,0LL));
        }
    }
    for (int n=0;n<=60;++n) for(int b:{1,3,8,65}) {
        std::vector<long long>a(n,0);BlockAddPoint blocks(a,b);
        for(int k=0;k<300;++k) {
            int l=rng()%(n+1),r=rng()%(n+1);if(l>r)std::swap(l,r);
            long long x=int(rng()%41)-20;
            if(n && rng()%3==0){int i=rng()%n;blocks.set(i,x);a[i]=x;}
            else{blocks.add(l,r,x);for(int i=l;i<r;++i)a[i]+=x;}
            for(int i=0;i<n;++i)assert(blocks.get(i)==a[i]);
        }
    }
    for (int run=0;run<500;++run) {
        int n=1+rng()%80,b=1+rng()%20;
        std::vector<int>a(n);
        for (int& x:a)x=1+rng()%30;
        JumpBlocks jumps(a,b);
        for (int k=0;k<150;++k) {
            int i=rng()%n,x=1+rng()%100; a[i]=x;jumps.set(i,x);
            for(int t=0;t<4;++t) {
                int start=rng()%n,final=start,count=0;
                for (int j=start;j<n;j+=a[j]) {final=j;++count;}
                assert(jumps.exit_from(start)==std::make_pair(final,count));
            }
        }
    }
    assert((JumpBlocks({1,2,1,1,2,1,1,3},3).exit_from(0)==std::make_pair(7,6)));
    for(int run=0;run<1500;++run) {
        int n=rng()%17;std::vector<long long>a(n);for(auto& x:a)x=int(rng()%15)-7;
        long long target=int(rng()%41)-20,expected=0;
        for(int mask=0;mask<(1<<n);++mask) {
            long long sum=0;for(int i=0;i<n;++i)if(mask>>i&1)sum+=a[i];
            expected+=sum==target;
        }
        assert(count_subsets(a,target)==expected);
    }
    assert(count_subsets(std::vector<long long>(40,0),0)==(1LL<<40));
    assert(count_subsets({1,1,2,3},3)==3);
    for(int run=0;run<1000;++run) {
        int n=rng()%16;long long capacity=rng()%35,expected=0;
        std::vector<Item>a(n);for(auto& x:a)x={rng()%16,rng()%20};
        for(int mask=0;mask<(1<<n);++mask){long long w=0,v=0;
            for(int i=0;i<n;++i)if(mask>>i&1){w+=a[i].weight;v+=a[i].value;}
            if(w<=capacity)expected=std::max(expected,v);
        }
        assert(best_knapsack(a,capacity)==expected);
    }
    PersistentStack stack;
    std::vector<PersistentStack::Root> roots{0};
    std::vector<std::vector<long long>> values(1);
    std::size_t pushes=0;
    for(int k=0;k<5000;++k) {
        int parent=rng()%roots.size();auto ref=values[parent];auto root=roots[parent];
        if(ref.empty() || rng()%3) {long long x=int(rng()%100)-50;root=stack.push(root,x);ref.push_back(x);++pushes;}
        else{root=stack.pop(root);ref.pop_back();}
        roots.push_back(root);values.push_back(ref);
        for(int t=0;t<3;++t){int v=rng()%roots.size();auto r=roots[v];
            assert(stack.size(r)==values[v].size());
            for(auto it=values[v].rbegin();it!=values[v].rend();++it){assert(stack.top(r)==*it);r=stack.pop(r);}
            assert(stack.empty(r));
        }
        assert(stack.allocated()==pushes);
    }
    PersistentQueue queue;
    std::vector<PersistentQueue::Version> versions(1);
    std::vector<std::deque<long long>> queues(1);
    for(int k=0;k<2000;++k) {
        int parent=rng()%versions.size();auto v=versions[parent];auto ref=queues[parent];
        if(ref.empty() || rng()%3){long long x=rng()%100;v=queue.push(v,x);ref.push_back(x);}
        else{v=queue.pop(v);ref.pop_front();}
        versions.push_back(v);queues.push_back(ref);
        for(int t=0;t<3;++t){int j=rng()%versions.size();auto r=versions[j];
            for(auto x:queues[j]){assert(queue.front(r)==x);r=queue.pop(r);}
            assert(queue.empty(r));
        }
    }
    PersistentQueue counterexample;PersistentQueue::Version v;
    for(int i=0;i<128;++i)v=counterexample.push(v,i);
    auto before=counterexample.allocated();
    for(int j=0;j<20;++j){auto child=counterexample.pop(v);assert(counterexample.front(child)==1);}
    assert(counterexample.allocated()-before==20*128);
    int caught=0;
    try{stack.top(0);}catch(const std::out_of_range&){++caught;}
    try{stack.pop(0);}catch(const std::out_of_range&){++caught;}
    try{queue.front({});}catch(const std::out_of_range&){++caught;}
    try{queue.pop({});}catch(const std::out_of_range&){++caught;}
    assert(caught==4);
    std::cout << "PASS: blocks, block add/point, jumps, subset counts, knapsack, branching stack/queue, repeated reversal\n";
}
