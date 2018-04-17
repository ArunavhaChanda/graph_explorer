//Implementation of our graph using a vector to represent a map/matrix

#include "our_concepts.h"
#include <vector>
#include <optional>
#include <algorithm>
#include <map>
#include <cassert>
#include <iostream>
#include <set>
#include <climits>
#include <functional>

using namespace std;

//definition of vertex class to be used within graph
template <class T>
class vertex{
    map<int, int> &adj;
    const function<void(const T&,const T&)> update;
    T& _val;

public:
    const T& val;
    vertex(T& rval, function<void(const T&,const T&)> u, map<int, int> &radj):adj{radj}, update{u}, val{rval}, _val{rval}{}
    void operator =(const T& newval){
        update(val, newval);
        _val = newval;
    }

    map<int,int>::iterator begin(){
        return adj.begin();
    }

    map<int,int>::iterator end(){
        return adj.end();
    }

    const int& operator[](int i) const{
        return adj[i];
    }
};

template <class T>
class graph_base{
    map<int, map<int, int>> adj;
    vector<T> node;
protected:

    int push_back(const T &t){
        int pos = node.size();
        node.push_back(t);
        return pos;
    }

    vertex<T> get_vertex(int i, const function<void(const T&, const T&)> upd){
        assert(i < node.size());
        return vertex<T>(node[i], upd, adj[i]);
    }

public:
    graph_base(){}
    
    graph_base(int n){
        node.resize(n);
    }

    virtual vertex<T> operator[](int i) = 0;
    virtual int count(T& val) = 0;

    optional<int> get_node(T& val){
        ptrdiff_t pos = distance(node.begin(), find(node.begin(), node.end(), val));
        if (pos == node.size()) return nullopt;
        return pos;
    }

    void add_edge(int src, int target, int weight = 1){
        assert(target < node.size() && src < node.size());
        adj[src][target] = weight;
    }

    void erase_edge(int src, int target){
        assert(target < node.size() && src < node.size());
        if(adj.find(src) != adj.end()){
            adj[src].erase(target);
        }
    }

    const vector<T> &nodes() const{
        return node;
    }

    const map<int, map<int, int>> &edges() const{
        return adj;
    }

    size_t size(){
        return node.size();
    }
};

//general graph implementation for T that does not support < operator
template <class T>
class graph : public graph_base<T> {
public:
    graph():graph_base<T>() { }

    graph(int n):graph_base<T>(n){ }

    graph(initializer_list<T> inp):graph_base<T>(){
        for(auto &it : inp){
            if(auto t = get_node(it)) continue;
            graph_base<T>::push_back(it);
        }
    }

    int push_back(T &t){
        return graph_base<T>::push_back(t);
    }

    vertex<T> operator[](int i){
        auto upd = [&](const T& oldval, const T& newval){
            cout << "Updating value: " << oldval << ' ' << newval << endl;
        };
        return graph_base<T>::get_vertex(i, upd);
    }

    int count(T& val){
        if(auto t = get_node(val)) return 1;
        return 0;
    }
};

//graph class specialization for a T that supports < operator
template <LessThanComparable T>
class graph<T> : public graph_base<T> {
    map<int, map<int, int>> adj;
    vector<T> node;
    map<T, int> lookup;

public:
    graph():graph_base<T>(){ }

    graph(int n):graph_base<T>(n){ }

    graph(initializer_list<T> inp):graph_base<T>(){
        for(auto &it : inp){
            if(lookup.count(it)) continue;
            lookup[it] = graph_base<T>::push_back(it);
        }
    }

    int push_back(T &t){
        if(lookup.count(t) == 0) return graph_base<T>::push_back(t);
        return lookup[t];
    }

    vertex<T> operator[](int i){
        auto upd = [&](const T& oldval, const T& newval){
            cout << "Updating value: " << oldval << ' ' << newval << endl;
            lookup.erase(oldval);
            lookup[newval] = i;
        };
        return graph_base<T>::get_vertex(i, upd);
    }

    optional<int> get_node(T& val){
        auto it = lookup.find(val);
        if(it == lookup.end()) return nullopt;
        return it->second;
    }

    int count(T& val){
        return lookup.count(val);
    }
};
