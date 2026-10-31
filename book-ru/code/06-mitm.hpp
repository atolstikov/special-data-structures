#pragma once
#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

inline std::vector<long long> subset_sums(
        const std::vector<long long>& a,int l,int r) {
    assert(0<=l && l<=r && r<=int(a.size()) && r-l<=20);
    std::vector<long long> sums{0};
    for (int i=l; i<r; ++i) {
        auto old=sums.size();
        for (std::size_t j=0; j<old; ++j)
            sums.push_back(sums[j]+a[i]);
    }
    return sums;
}
inline long long count_subsets(const std::vector<long long>& a,
                               long long target) {
    assert(a.size()<=40);
    int n=int(a.size()), mid=n/2;
    auto left=subset_sums(a,0,mid);
    auto right=subset_sums(a,mid,n);
    std::sort(right.begin(),right.end());
    long long answer=0;
    for (long long x:left) {
        auto range=std::equal_range(right.begin(),right.end(),target-x);
        answer+=range.second-range.first;
    }
    return answer;
}

struct Item { long long weight,value; };
using State=std::pair<long long,long long>; // weight, value
inline std::vector<State> enumerate_states(const std::vector<Item>& a,
                                           int l,int r,long long capacity) {
    std::vector<State> states{{0,0}};
    for (int i=l; i<r; ++i) {
        auto old=states.size();
        for (std::size_t j=0; j<old; ++j) {
            auto [w,v]=states[j];
            if (a[i].weight<=capacity-w)
                states.push_back({w+a[i].weight,v+a[i].value});
        }
    }
    return states;
}
inline long long best_knapsack(const std::vector<Item>& a,
                               long long capacity) {
    assert(a.size()<=40 && capacity>=0);
    for (auto item:a) assert(item.weight>=0 && item.value>=0);
    int mid=int(a.size())/2;
    auto right=enumerate_states(a,mid,int(a.size()),capacity);
    std::sort(right.begin(),right.end(),[](State x,State y) {
        return x.first!=y.first ? x.first<y.first : x.second>y.second;
    });
    std::vector<State> frontier;
    long long best=-1;
    for (auto state:right) if (state.second>best) {
        frontier.push_back(state); best=state.second;
    }
    auto left=enumerate_states(a,0,mid,capacity);
    long long answer=0;
    for (auto [w,v]:left) {
        long long room=capacity-w;
        auto it=std::upper_bound(frontier.begin(),frontier.end(),room,
            [](long long limit,State s){return limit<s.first;});
        assert(it!=frontier.begin());
        --it; answer=std::max(answer,v+it->second);
    }
    return answer;
}
