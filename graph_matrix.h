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

//general graph implementation for T that does not support < operator
template <class T>
class mgraph{
    map<int, map<int, int>> adj;
    vector<T> node;

public:
    mgraph(){}

    mgraph(int n){
        node.resize(n);
    }

    mgraph(initializer_list<T> inp){
        for(auto it=inp.begin();it!=inp.end();it++){
            if(find(node.begin(), node.end(), *it) == node.end()){
                node.push_back(*it);
            }
        }
    }

    int push_back(T &t){
        node.push_back(t);
        return node.size() - 1;
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

    vertex<T> operator[](int i){
        assert(i < node.size());
        auto upd = [&](const T& oldval, const T& newval){
            cout << "Updating value: " << oldval << ' ' << newval << endl;
        };
        return vertex<T>(node[i], upd,adj[i]);
    }

    optional<int> get_node(T& val){
        ptrdiff_t pos = distance(node.begin(), find(node.begin(), node.end(), val));
        if (pos >= node.size()) return nullopt;
        return pos;
    }

    int count(T& val){
        if(find(node.begin(), node.end(), val) != node.end()) {return 1;}
        return 0;
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

//graph class specialization for a T that supports < operator
template <LessThanComparable T>
class mgraph<T>{
    map<int, map<int, int>> adj;
    vector<T> node;
    map<T, int> lookup;

public:
    mgraph(){}

    mgraph(int n){
        node.resize(n);
    }

    mgraph(initializer_list<T> inp){
        for(auto it=inp.begin();it!=inp.end();it++){
            if(lookup.find(*it) == lookup.end()){
                lookup[*it] = node.size();
                node.push_back(*it);
            }
        }
    }

    int push_back(T &t){
        node.push_back(t);
        return node.size() - 1;
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

    vertex<T> operator[](int i){
        assert(i < node.size());
        auto upd = [&](const T& oldval, const T& newval){
            cout << "Updating value: " << oldval << ' ' << newval << endl;
            lookup.erase(oldval);
            lookup[newval] = i;
        };
        return vertex<T>(node[i], upd,adj[i]);
    }

    optional<int> get_node(T& val){
        auto it = lookup.find(val);
        if(it == lookup.end()) return nullopt;
        return it->second;
    }

    int count(T& val){
        return lookup.count(val);
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
