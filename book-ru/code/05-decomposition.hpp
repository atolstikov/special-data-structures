#pragma once
#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

class BlockSum {
    int n, width;
    std::vector<long long> a, total;
public:
    BlockSum(std::vector<long long> values, int b)
        : n(int(values.size())), width(b), a(std::move(values)) {
        assert(0 <= n && n <= 1000000 && width > 0);
        total.assign(n / width + (n % width != 0), 0);
        for (int i=0; i<n; ++i) total[i/width] += a[i];
    }
    void set(int i, long long x) {
        assert(0 <= i && i < n);
        total[i/width] += x-a[i];
        a[i] = x;
    }
    long long sum(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        long long answer=0;
        while (l<r && l%width) answer += a[l++];
        while (width <= r-l) {
            answer += total[l/width]; l += width;
        }
        while (l<r) answer += a[l++];
        return answer;
    }
};

class JumpBlocks {
    int n, width;
    std::vector<int> a, next, count, last;
    void rebuild(int block) {
        int l=block*width, r=std::min(n,l+width);
        for (int i=r-1; i>=l; --i) {
            long long j=1LL*i+a[i];
            if (j>=r) {
                next[i]=int(std::min<long long>(n,j));
                count[i]=1; last[i]=i;
            } else {
                next[i]=next[int(j)];
                count[i]=1+count[int(j)];
                last[i]=last[int(j)];
            }
        }
    }
public:
    JumpBlocks(std::vector<int> jumps, int b)
        : n(int(jumps.size())), width(b), a(std::move(jumps)),
          next(n), count(n), last(n) {
        assert(n<=1000000 && 1<=width && width<=1000000);
        for (int x:a) assert(1<=x && x<=1000000000);
        for (int l=0; l<n; l+=width) rebuild(l/width);
    }
    void set(int i,int x) {
        assert(0<=i && i<n && 1<=x && x<=1000000000);
        a[i]=x; rebuild(i/width);
    }
    std::pair<int,int> exit_from(int i) const {
        assert(0<=i && i<n);
        int steps=0, final=i;
        while (i<n) {
            steps+=count[i]; final=last[i]; i=next[i];
        }
        return {final,steps};
    }
};


class BlockAddPoint {
    int n, width;
    std::vector<long long> base, lazy;
public:
    BlockAddPoint(std::vector<long long> a,int b)
        : n(int(a.size())), width(b), base(std::move(a)) {
        assert(n<=1000000 && width>0);
        lazy.assign(n/width+(n%width!=0),0);
    }
    void add(int l,int r,long long x) {
        assert(0<=l && l<=r && r<=n);
        while(l<r && l%width) base[l++]+=x;
        while(width<=r-l) {lazy[l/width]+=x; l+=width;}
        while(l<r) base[l++]+=x;
    }
    long long get(int i) const {
        assert(0<=i && i<n); return base[i]+lazy[i/width];
    }
    void set(int i,long long x) {
        assert(0<=i && i<n); base[i]=x-lazy[i/width];
    }
};
