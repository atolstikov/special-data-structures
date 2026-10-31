#include "11-skip-list.hpp"
#include <cassert>
#include <climits>
#include <iostream>
#include <set>
#include <random>
#include <algorithm>

int main() {
    for (unsigned seed=0;seed<20;++seed) {
        SkipSet skip(seed);
        std::set<int> model;
        std::mt19937 gen(seed+1000);
        auto check=[&]() {
            assert(skip.valid());
            assert(skip.size()==model.size());
            assert(skip.values()==std::vector<int>(model.begin(),model.end()));
        };
        assert(!skip.lower_bound(INT_MIN));
        assert(!skip.erase(0));
        for (int key:{INT_MIN,INT_MAX,0,-1,1}) {
            assert(skip.insert(key)==model.insert(key).second);
            assert(!skip.insert(key));
        }
        check();
        for (int step=0;step<15000;++step) {
            int x=static_cast<int>(gen()%1001)-500;
            int op=static_cast<int>(gen()%4);
            if (op==0) assert(skip.insert(x)==model.insert(x).second);
            else if (op==1) assert(skip.erase(x)==(model.erase(x)!=0));
            else if (op==2) assert(skip.contains(x)==(model.count(x)!=0));
            else {
                auto actual=skip.lower_bound(x), expected=std::optional<int>{};
                auto it=model.lower_bound(x);
                if (it!=model.end()) expected=*it;
                assert(actual==expected);
            }
            if (step%100==0) check();
        }
        for (int x=-501;x<=501;++x) {
            auto actual=skip.lower_bound(x), expected=std::optional<int>{};
            auto it=model.lower_bound(x);
            if (it!=model.end()) expected=*it;
            assert(actual==expected);
        }
        auto keys=skip.values();
        std::shuffle(keys.begin(),keys.end(),gen);
        for (int x:keys) {assert(skip.erase(x));model.erase(x);}
        check();assert(skip.size()==0);
        for (int x=0;x<2000;++x) assert(skip.insert(x));
        assert(skip.valid());
        for (int x=1999;x>=0;--x) assert(skip.erase(x));
        check();
        // Destructor also sees a non-empty list.
        for (int x=2000;x>=0;--x) assert(skip.insert(x));
        assert(skip.valid());
    }
    std::cout << "PASS: 20 seeds, 300000 mixed operations, bounds, invariants, reuse\n";
}
