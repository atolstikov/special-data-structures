#pragma once
#include <cassert>
#include <vector>

class LazySumTree {
    struct Node {
        long long sum = 0, mul = 1, add = 0;
    };
    int n;
    std::vector<Node> t;
    void build(int v, int l, int r, const std::vector<long long>& a) {
        if (r - l == 1) { t[v].sum = a[l]; return; }
        int m = l + (r - l) / 2;
        build(v * 2, l, m, a);
        build(v * 2 + 1, m, r, a);
        pull(v);
    }
    void pull(int v) { t[v].sum = t[v*2].sum + t[v*2+1].sum; }
    // Apply NEW transformation x -> a*x+b after the stored one.
    void apply(int v, int len, long long a, long long b) {
        t[v].sum = a * t[v].sum + b * len;
        t[v].mul = a * t[v].mul;
        t[v].add = a * t[v].add + b;
    }
    void push(int v, int l, int r) {
        int m = l + (r - l) / 2;
        apply(v*2, m-l, t[v].mul, t[v].add);
        apply(v*2+1, r-m, t[v].mul, t[v].add);
        t[v].mul = 1; t[v].add = 0;
    }
    void update(int v, int l, int r, int ql, int qr,
                long long a, long long b) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) { apply(v, r-l, a, b); return; }
        push(v, l, r);
        int m = l + (r-l)/2;
        update(v*2, l, m, ql, qr, a, b);
        update(v*2+1, m, r, ql, qr, a, b);
        pull(v);
    }
    long long query(int v, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) return 0;
        if (ql <= l && r <= qr) return t[v].sum;
        push(v, l, r);
        int m = l + (r-l)/2;
        return query(v*2, l, m, ql, qr)
             + query(v*2+1, m, r, ql, qr);
    }
    void bounds(int l, int r) const { assert(0 <= l && l <= r && r <= n); }
public:
    explicit LazySumTree(const std::vector<long long>& a)
        : n(int(a.size())), t(4 * (n > 0 ? n : 1)) {
        if (n) build(1, 0, n, a);
    }
    void add(int l, int r, long long x) {
        bounds(l,r);
        if (l < r) update(1,0,n,l,r,1,x);
    }
    void set(int l, int r, long long x) {
        bounds(l,r);
        if (l < r) update(1,0,n,l,r,0,x);
    }
    long long sum(int l, int r) {
        bounds(l,r);
        return l == r ? 0 : query(1,0,n,l,r);
    }
};
