#pragma once
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class HistoricalCell {
    std::vector<std::pair<int,long long>> history;
public:
    explicit HistoricalCell(long long initial):history{{0,initial}} {}
    void write(int time,long long value) {
        if (time<=history.back().first)
            throw std::invalid_argument("time must increase");
        history.emplace_back(time,value);
    }
    long long read(int time) const {
        if (time<0) throw std::out_of_range("negative time");
        auto it=std::upper_bound(history.begin(),history.end(),time,
            [](int t,const auto& entry){ return t<entry.first; });
        return std::prev(it)->second;
    }
};

// One rooted ordered tree, encoded by entry '(' and exit ')'.
inline std::vector<std::vector<int>> decode(const std::string& s) {
    if (s.empty()) throw std::invalid_argument("empty code");
    std::vector<std::vector<int>> children;
    std::vector<int> stack;
    for (char c:s) {
        if (c=='(') {
            if (stack.empty() && !children.empty())
                throw std::invalid_argument("multiple roots");
            int v=static_cast<int>(children.size());
            children.emplace_back();
            if (!stack.empty()) children[stack.back()].push_back(v);
            stack.push_back(v);
        } else if (c==')') {
            if (stack.empty()) throw std::invalid_argument("extra close");
            stack.pop_back();
        } else throw std::invalid_argument("unknown token");
    }
    if (!stack.empty()) throw std::invalid_argument("unclosed node");
    return children;
}

struct EulerIndex { std::vector<int> tin,tout,depth; };
// Precondition: children describes one tree rooted at 0.
inline EulerIndex index_tree(const std::vector<std::vector<int>>& children) {
    int n=static_cast<int>(children.size());
    if (!n) throw std::invalid_argument("empty tree");
    EulerIndex e{std::vector<int>(n),std::vector<int>(n),
                 std::vector<int>(n)};
    std::vector<std::pair<int,std::size_t>> stack{{0,0}};
    int timer=0;
    e.tin[0]=timer++;
    while (!stack.empty()) {
        int v=stack.back().first;
        std::size_t next=stack.back().second;
        if (next==children[v].size()) {
            e.tout[v]=timer;
            stack.pop_back();
        } else {
            ++stack.back().second;
            int u=children[v][next];
            e.depth[u]=e.depth[v]+1;
            e.tin[u]=timer++;
            stack.emplace_back(u,0);
        }
    }
    return e;
}
inline bool ancestor(const EulerIndex& e,int u,int v) {
    return e.tin[u]<=e.tin[v] && e.tin[v]<e.tout[u];
}
