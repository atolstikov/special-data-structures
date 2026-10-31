#pragma once
#include <cassert>
#include <cstddef>
#include <deque>
#include <stdexcept>

class PersistentSum {
public:
    using Root = std::size_t;
private:
    struct Node { long long sum=0; Root left=0, right=0; };
    std::deque<Node> pool{Node{}};
    int n;
    Root change(Root r, int lo, int hi, int pos, long long x) {
        Node t=pool[r];
        if (hi-lo==1) t.sum=x;
        else {
            int mid=lo+(hi-lo)/2;
            if (pos<mid) t.left=change(t.left,lo,mid,pos,x);
            else t.right=change(t.right,mid,hi,pos,x);
            t.sum=pool[t.left].sum+pool[t.right].sum;
        }
        pool.push_back(t);
        return pool.size()-1;
    }
    long long query(Root r,int lo,int hi,int l,int h) const {
        if (!r || h<=lo || hi<=l) return 0;
        if (l<=lo && hi<=h) return pool[r].sum;
        int mid=lo+(hi-lo)/2;
        return query(pool[r].left,lo,mid,l,h)
             + query(pool[r].right,mid,hi,l,h);
    }
public:
    explicit PersistentSum(int size): n(size) { assert(n>0); }
    Root set(Root r,int pos,long long x) {
        assert(r<pool.size() && 0<=pos && pos<n);
        return change(r,0,n,pos,x);
    }
    long long sum(Root r,int l,int h) const {
        assert(r<pool.size() && l>=0 && h>=0 && h<=n);
        assert(static_cast<unsigned>(l)<=static_cast<unsigned>(h));
        return query(r,0,n,l,h);
    }
    Root add(Root r,int pos,long long delta) {
        assert(0<=pos && pos<n);
        return set(r,pos,sum(r,pos,pos+1)+delta);
    }
    // Precondition: every leaf of after-before is nonnegative.
    int kth(Root before,Root after,long long k) const {
        assert(before<pool.size() && after<pool.size());
        long long total=pool[after].sum-pool[before].sum;
        if (k<1 || k>total) throw std::out_of_range("rank");
        int lo=0,hi=n;
        while (hi-lo>1) {
            int mid=lo+(hi-lo)/2;
            Root a=pool[before].left, b=pool[after].left;
            long long count=pool[b].sum-pool[a].sum;
            if (k<=count) { before=a; after=b; hi=mid; }
            else {
                k-=count;
                before=pool[before].right;
                after=pool[after].right;
                lo=mid;
            }
        }
        return lo;
    }
    std::size_t allocated() const { return pool.size()-1; }
};
