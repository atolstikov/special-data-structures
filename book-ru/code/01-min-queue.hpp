#pragma once
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

class MinStack {
    std::vector<std::pair<long long, long long>> a;
public:
    bool empty() const { return a.empty(); }
    void push(long long x) {
        long long m = empty() ? x : std::min(x, a.back().second);
        a.emplace_back(x, m);
    }
    long long top() const {
        if (empty()) throw std::out_of_range("empty stack");
        return a.back().first;
    }
    long long minimum() const {
        if (empty()) throw std::out_of_range("empty stack");
        return a.back().second;
    }
    void pop() {
        if (empty()) throw std::out_of_range("empty stack");
        a.pop_back();
    }
};

class MinQueue {
    MinStack in, out;
    void prepare() {
        if (out.empty()) {
            while (!in.empty()) {
                long long x = in.top();
                out.push(x);
                in.pop();
            }
        }
    }
public:
    bool empty() const { return in.empty() && out.empty(); }
    void push(long long x) { in.push(x); }
    long long front() {
        prepare();
        return out.top();
    }
    void pop() {
        prepare();
        out.pop();
    }
    long long minimum() const {
        if (in.empty()) return out.minimum();
        if (out.empty()) return in.minimum();
        return std::min(in.minimum(), out.minimum());
    }
};
