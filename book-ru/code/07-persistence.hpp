#pragma once
#include <cassert>
#include <cstddef>
#include <deque>
#include <stdexcept>

class PersistentStack {
public:
    using Root=std::size_t;
private:
    struct Node { long long value; Root next; std::size_t length; };
    std::deque<Node> pool{{0,0,0}}; // index 0 is the empty stack
public:
    bool empty(Root r) const { assert(r<pool.size()); return r==0; }
    std::size_t size(Root r) const {
        assert(r<pool.size()); return pool[r].length;
    }
    Root push(Root r,long long value) {
        assert(r<pool.size());
        pool.push_back({value,r,pool[r].length+1});
        return pool.size()-1;
    }
    long long top(Root r) const {
        if (empty(r)) throw std::out_of_range("empty persistent stack");
        return pool[r].value;
    }
    Root pop(Root r) const {
        if (empty(r)) throw std::out_of_range("empty persistent stack");
        return pool[r].next;
    }
    std::size_t allocated() const { return pool.size()-1; }
};

// Correct persistent queue, but NOT constant amortized time over a version tree.
class PersistentQueue {
public:
    struct Version { PersistentStack::Root front=0, back=0; };
private:
    PersistentStack stack;
    Version normalize(Version v) {
        if (!stack.empty(v.front)) return v;
        while (!stack.empty(v.back)) {
            v.front=stack.push(v.front,stack.top(v.back));
            v.back=stack.pop(v.back);
        }
        return v;
    }
public:
    bool empty(Version v) const {
        return stack.empty(v.front) && stack.empty(v.back);
    }
    Version push(Version v,long long x) {
        v.back=stack.push(v.back,x); return v;
    }
    long long front(Version v) {
        v=normalize(v); return stack.top(v.front);
    }
    Version pop(Version v) {
        v=normalize(v); v.front=stack.pop(v.front); return v;
    }
    std::size_t allocated() const { return stack.allocated(); }
};
