#pragma once
#include <cassert>
#include <vector>

class SumTree {
    int n, base = 1;
    std::vector<long long> t;
public:
    explicit SumTree(const std::vector<long long>& a) : n(int(a.size())) {
        while (base < n) base *= 2;
        t.assign(2 * base, 0);
        for (int i = 0; i < n; ++i) t[base + i] = a[i];
        for (int v = base - 1; v > 0; --v)
            t[v] = t[2 * v] + t[2 * v + 1];
    }
    void set(int p, long long x) {
        assert(0 <= p && p < n);
        p += base;
        t[p] = x;
        while (p > 1) {
            p /= 2;
            t[p] = t[2 * p] + t[2 * p + 1];
        }
    }
    long long sum(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        long long left = 0, right = 0;
        l += base; r += base;
        while (l < r) {
            if (l & 1) left = left + t[l++];
            if (r & 1) right = t[--r] + right;
            l /= 2; r /= 2;
        }
        return left + right;
    }
};
