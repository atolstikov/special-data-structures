#pragma once
#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

class PersistentBST {
    struct Node;
public:
    using Root=std::shared_ptr<const Node>;
private:
    struct Node {
        int key; Root left,right;
        Node(int k,Root a,Root b):key(k),left(a),right(b) {}
    };
    static Root make(int k,Root a,Root b) {
        return std::make_shared<const Node>(k,a,b);
    }
public:
    static bool contains(Root r,int x) {
        while (r) {
            if (x==r->key) return true;
            r=x<r->key ? r->left : r->right;
        }
        return false;
    }
    static Root insert(Root r,int x) {
        if (!r) return make(x,nullptr,nullptr);
        if (x==r->key) return r;
        if (x<r->key) {
            Root a=insert(r->left,x);
            if (a==r->left) return r;
            return make(r->key,a,r->right);
        }
        Root b=insert(r->right,x);
        if (b==r->right) return r;
        return make(r->key,r->left,b);
    }
    static Root erase(Root r,int x) {
        if (!r) return r;
        if (x<r->key) {
            Root a=erase(r->left,x);
            if (a==r->left) return r;
            return make(r->key,a,r->right);
        }
        if (x>r->key) {
            Root b=erase(r->right,x);
            if (b==r->right) return r;
            return make(r->key,r->left,b);
        }
        if (!r->left) return r->right;
        if (!r->right) return r->left;
        Root s=r->right;
        while (s->left) s=s->left;
        return make(s->key,r->left,erase(r->right,s->key));
    }
};

class PersistentLeftist {
    struct Node;
public:
    using Root=std::shared_ptr<const Node>;
private:
    struct Node {
        long long key; Root left,right; int rank;
        Node(long long x,Root a,Root b,int d)
            :key(x),left(a),right(b),rank(d) {}
    };
    static int rank(Root r) { return r ? r->rank : 0; }
    static Root make(long long x,Root a,Root b) {
        if (rank(a)<rank(b)) std::swap(a,b);
        int d=rank(b)+1;
        return std::make_shared<const Node>(x,a,b,d);
    }
public:
    static Root meld(Root a,Root b) {
        if (!a) return b;
        if (!b) return a;
        if (b->key<a->key) std::swap(a,b);
        return make(a->key,a->left,meld(a->right,b));
    }
    static Root push(Root r,long long x) {
        return meld(r,make(x,nullptr,nullptr));
    }
    static long long top(Root r) {
        if (!r) throw std::out_of_range("empty heap");
        return r->key;
    }
    static Root pop(Root r) {
        if (!r) throw std::out_of_range("empty heap");
        return meld(r->left,r->right);
    }
    static bool valid(Root r) {
        if (!r) return true;
        return rank(r->left)>=rank(r->right)
            && r->rank==rank(r->right)+1
            && (!r->left || r->key<=r->left->key)
            && (!r->right || r->key<=r->right->key)
            && valid(r->left) && valid(r->right);
    }
};
