#include "08-persistent-segment.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<long long> a{5,1,5,3,1}, c=a;
    std::sort(c.begin(),c.end());
    c.erase(std::unique(c.begin(),c.end()),c.end());
    PersistentSum tree(static_cast<int>(c.size()));
    std::vector<PersistentSum::Root> prefix{0};
    for (long long x:a) {
        int rank=static_cast<int>(
            std::lower_bound(c.begin(),c.end(),x)-c.begin());
        prefix.push_back(tree.add(prefix.back(),rank,1));
    }
    int l=1,r=5;
    int low=static_cast<int>(
        std::lower_bound(c.begin(),c.end(),1)-c.begin());
    int high=static_cast<int>(
        std::lower_bound(c.begin(),c.end(),5)-c.begin());
    std::cout << tree.sum(prefix[r],low,high)
               -tree.sum(prefix[l],low,high) << '\n';
    int rank=tree.kth(prefix[l],prefix[r],3);
    std::cout << c[rank] << '\n';
}
