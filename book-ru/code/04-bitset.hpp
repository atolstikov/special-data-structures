#pragma once
#include <cassert>
#include <cstdint>
#include <vector>
#include <utility>

class Bits {
    int n;
    std::vector<std::uint64_t> w;
    void trim() {
        if (n % 64) w.back() &= (std::uint64_t{1} << (n % 64)) - 1;
    }
public:
    explicit Bits(int size) : n(size), w((size + 63) / 64, 0) { assert(size >= 0); }
    int size() const { return n; }
    bool test(int x) const {
        assert(0 <= x && x < n);
        return (w[x/64] >> (x%64)) & 1;
    }
    void set(int x) {
        assert(0 <= x && x < n);
        w[x/64] |= std::uint64_t{1} << (x%64);
    }
    void reset(int x) {
        assert(0 <= x && x < n);
        w[x/64] &= ~(std::uint64_t{1} << (x%64));
    }
    void unite(const Bits& b) {
        assert(n == b.n);
        for (std::size_t i=0; i<w.size(); ++i) w[i] |= b.w[i];
    }
    void intersect(const Bits& b) {
        assert(n == b.n);
        for (std::size_t i=0; i<w.size(); ++i) w[i] &= b.w[i];
    }
    void subtract(const Bits& b) {
        assert(n == b.n);
        for (std::size_t i=0; i<w.size(); ++i) w[i] &= ~b.w[i];
    }
    void xor_with(const Bits& b) {
        assert(n == b.n);
        for (std::size_t i=0; i<w.size(); ++i) w[i] ^= b.w[i];
    }
    int count() const {
        int result=0;
        for (auto x:w) while (x) { x &= x-1; ++result; }
        return result;
    }
    Bits shifted(int d) const {
        assert(d >= 0);
        Bits b(n);
        if (d >= n) return b;
        int whole=d/64, part=d%64;
        for (int i=0; i<int(w.size()); ++i) {
            int j=i+whole;
            if (j<int(w.size())) b.w[j] |= w[i] << part;
            if (part && j+1<int(w.size()))
                b.w[j+1] |= w[i] >> (64-part);
        }
        b.trim();
        return b;
    }
};

inline std::vector<Bits> closure(std::vector<Bits> reach) {
    int n=int(reach.size());
    for (int i=0; i<n; ++i) { assert(reach[i].size()==n); reach[i].set(i); }
    for (int k=0; k<n; ++k)
        for (int i=0; i<n; ++i)
            if (reach[i].test(k)) reach[i].unite(reach[k]);
    return reach;
}

inline std::vector<int> bit_bfs(const std::vector<Bits>& adj, int s) {
    int n=int(adj.size());
    assert(0 <= s && s < n);
    for (const auto& row:adj) assert(row.size()==n);
    Bits seen(n);
    seen.set(s);
    std::vector<int> frontier{s}, dist(n,-1);
    dist[s]=0;
    int depth=0;
    while (!frontier.empty()) {
        Bits next(n);
        for (int v:frontier) next.unite(adj[v]);
        next.subtract(seen);
        seen.unite(next);
        frontier.clear();
        ++depth;
        for (int v=0; v<n; ++v)
            if (next.test(v)) { frontier.push_back(v); dist[v]=depth; }
    }
    return dist;
}

inline int gf2_rank(std::vector<Bits> rows, int columns) {
    int rank=0, m=int(rows.size());
    for (const auto& row:rows) assert(row.size()==columns);
    for (int c=0; c<columns && rank<m; ++c) {
        int p=rank;
        while (p<m && !rows[p].test(c)) ++p;
        if (p==m) continue;
        std::swap(rows[p],rows[rank]);
        for (int i=rank+1; i<m; ++i)
            if (rows[i].test(c)) rows[i].xor_with(rows[rank]);
        ++rank;
    }
    return rank;
}
