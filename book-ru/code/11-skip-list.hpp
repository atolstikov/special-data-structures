#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <random>
#include <vector>

class SkipSet {
    static constexpr int MAX_LEVEL=32;
    struct Node {
        int key;
        std::vector<Node*> next;
        Node(int x,int height):key(x),next(height,nullptr) {}
    };
    Node head{0,MAX_LEVEL};
    int levels=1;
    std::size_t count=0;
    std::mt19937_64 random;

    int random_height() {
        int h=1;
        while (h<MAX_LEVEL && (random()&1ULL)) ++h;
        return h;
    }
    Node* predecessors(int x,std::array<Node*,MAX_LEVEL>& before) {
        Node* p=&head;
        for (int i=levels-1;i>=0;--i) {
            while (p->next[i] && p->next[i]->key<x)
                p=p->next[i];
            before[i]=p;
        }
        return p->next[0];
    }
public:
    explicit SkipSet(std::uint64_t seed=20261031):random(seed) {}
    SkipSet(const SkipSet&)=delete;
    SkipSet& operator=(const SkipSet&)=delete;
    ~SkipSet() {
        Node* p=head.next[0];
        while (p) {
            Node* next=p->next[0];
            delete p;
            p=next;
        }
    }
    std::size_t size() const { return count; }
    std::optional<int> lower_bound(int x) const {
        const Node* p=&head;
        for (int i=levels-1;i>=0;--i)
            while (p->next[i] && p->next[i]->key<x)
                p=p->next[i];
        p=p->next[0];
        if (!p) return std::nullopt;
        return p->key;
    }
    bool contains(int x) const {
        auto answer=lower_bound(x);
        return answer && *answer==x;
    }
    bool insert(int x) {
        std::array<Node*,MAX_LEVEL> before{};
        Node* candidate=predecessors(x,before);
        if (candidate && candidate->key==x) return false;
        int h=random_height();
        Node* fresh=new Node(x,h);
        for (int i=levels;i<h;++i) before[i]=&head;
        for (int i=0;i<h;++i) {
            fresh->next[i]=before[i]->next[i];
            before[i]->next[i]=fresh;
        }
        if (h>levels) levels=h;
        ++count;
        return true;
    }
    bool erase(int x) {
        std::array<Node*,MAX_LEVEL> before{};
        Node* candidate=predecessors(x,before);
        if (!candidate || candidate->key!=x) return false;
        int h=static_cast<int>(candidate->next.size());
        for (int i=0;i<h;++i)
            before[i]->next[i]=candidate->next[i];
        delete candidate;
        --count;
        while (levels>1 && !head.next[levels-1]) --levels;
        return true;
    }
    // Full traversal: used by the tests, not by fast operations.
    std::vector<int> values() const {
        std::vector<int> result;
        for (const Node* p=head.next[0];p;p=p->next[0])
            result.push_back(p->key);
        return result;
    }
    bool valid() const {
        std::vector<const Node*> base;
        for (const Node* p=head.next[0];p;p=p->next[0]) {
            if (base.size()>=count || p->next.empty()
                || p->next.size()>MAX_LEVEL) return false;
            if (!base.empty() && base.back()->key>=p->key) return false;
            base.push_back(p);
        }
        if (base.size()!=count || levels<1 || levels>MAX_LEVEL)
            return false;
        int maximum=1;
        for (const Node* p:base)
            if (static_cast<int>(p->next.size())>maximum)
                maximum=static_cast<int>(p->next.size());
        if (levels!=maximum) return false;
        for (int i=0;i<MAX_LEVEL;++i) {
            const Node* p=head.next[i];
            for (const Node* expected:base)
                if (static_cast<int>(expected->next.size())>i) {
                    if (p!=expected) return false;
                    p=p->next[i];
                }
            if (p) return false;
        }
        return true;
    }
};
