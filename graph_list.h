//Implementation of our graph using a vector to represent a map/matrix

#include "our_concepts.h"
#include <vector>
#include <optional>
#include <algorithm>
#include <map>
#include <cassert>
#include <iostream>
#include <list>
#include <set>
#include <climits>
#include <functional>

#ifndef GRAPH_LIST
#define GRAPH_LIST 1

namespace graphlist{
    using namespace std;

    struct duplicate_vertex_error : public exception {
       const char * what () const throw () {
          return "the given vertex already exists";
       }
    };

    //definition of vertex class to be used within graph
    template <class T, class Edge>
    class vertex{
        list<pair<int, Edge>> &adj;
        const function<void(const T&,const T&)> update;
        T& _val;

    public:
        const T& val;
        vertex(T& rval, function<void(const T&,const T&)> u, list<pair<int, Edge>> &radj):adj{radj}, update{u}, val{rval}, _val{rval}{}
        void operator =(const T& newval){
            update(val, newval);
            _val = newval;
        }

        typename list<pair<int, Edge>>::iterator begin(){
            return adj.begin();
        }

        typename list<pair<int, Edge>>::iterator end(){
            return adj.end();
        }

        int& operator[](int i) {
            for(auto &p : adj) if(p.first == i) return p.second;
            adj.push_back({i,0});
            return adj.back().second;
        }
    };

    // Graph that just stores nodes (no equal to or less than operator required for T)
    template <class T, class Edge>
    class unordered_graph_base{
        map<int, list<pair<int, Edge>>> adj;
        vector<T> node;
    protected:
        unordered_graph_base() { }
        
        unordered_graph_base(int n){
            node.resize(n);
        }

        unordered_graph_base(const initializer_list<T> &inp){
            node = vector<T>(inp.begin(), inp.end());
        }

        vertex<T, Edge> get_vertex(int i, const function<void(const T&, const T&)> upd){
            assert(i < node.size());
            return vertex<T, Edge>(node[i], upd, adj[i]);
        }

    public:
        unordered_graph_base(const unordered_graph_base<T, Edge> &val):
            node(val.node),
            adj(val.adj)
            {}

        unordered_graph_base& operator= (const unordered_graph_base<T, Edge> &val){
            if (this != &val)  {            
                node = val.node;  
                adj = val.adj;
            }  
            return *this;  
        }  

        virtual vertex<T, Edge> operator[](int i) = 0;

        int push_back(const T &t){
            int pos = node.size();
            node.push_back(t);
            return pos;
        }

        void add_edge(int src, int target, const Edge &weight){
            assert(target < node.size() && src < node.size());
            bool found = false;
            for(auto &p : adj[src]){
                if(p.first == target) { found = true; p.second = weight; break; }
            }
            if(!found) adj[src].push_back({target, weight});
        }

        void add_undirected_edge(int src, int target, const Edge &weight){
            add_edge(src, target, weight);
            add_edge(target, src, weight);
        }

        void erase_edge(int src, int target){
            assert(target < node.size() && src < node.size());
            for(auto it = adj[src].begin();it != adj[src].end();++it)
                if(it->first == target){
                    adj[src].erase(it); break;
                }
        }

        void erase_undirected_edge(uint32_t src, uint32_t target){
            erase_edge(src, target);
            erase_edge(target, src);
        }        

        const vector<T> &nodes() const{
            return node;
        }

        size_t size(){
            return node.size();
        }
    };

    // Graph that stores nodes that support == operator and does not allow duplicates.
    template <EqualityComparable T, class Edge>
    class graph_base : public unordered_graph_base<T, Edge> {
    protected:
        graph_base(): unordered_graph_base<T, Edge>() { }
        
        graph_base(int n) : unordered_graph_base<T, Edge>(n){ }
    public:
        int count(T& val){
            if(auto t = get_index(val)) return 1;
            return 0;
        }

        const uint32_t count(const T& val)const{
            if(auto t = get_index(val)) return 1;
            return 0;
        }
        
        virtual optional<int> get_index(const T& val) = 0;
    };

    //general graph implementation for T that does not support < operator
    template <EqualityComparable T, class Edge>
    class graph : public graph_base<T, Edge> {
    public:

        graph():graph_base<T, Edge>() { }

        graph(int n):graph_base<T, Edge>(n){ }

        graph(const initializer_list<T> &inp):graph_base<T, Edge>(){
            for(auto &it : inp){
                if(auto t = get_index(it)) continue;
                graph_base<T, Edge>::push_back(it);
            }
        }

        optional<int> get_index(const T& val){
            auto &node = unordered_graph_base<T, Edge>::nodes();
            ptrdiff_t pos = distance(node.begin(), find(node.begin(), node.end(), val));
            if (pos == node.size()) return nullopt;
            return pos;
        }

        int push_back(T &t){
            return graph_base<T, Edge>::push_back(t);
        }

        vertex<T, Edge> operator[](int i){
            auto upd = [&](const T& oldval, const T& newval){
                if(!(oldval == newval)){
                    if(auto t = this->get_index(newval)) throw duplicate_vertex_error();
                }
            };
            return graph_base<T, Edge>::get_vertex(i, upd);
        }
    };

    //graph class specialization for a T that supports < operator
    template <LessThanComparable T, class Edge>
    class graph<T, Edge> : public graph_base<T, Edge> {
        map<T, int> lookup;
    public:
        graph():graph_base<T, Edge>(){ }

        graph(int n):graph_base<T, Edge>(n){ }

        graph(const initializer_list<T> &inp):graph_base<T, Edge>(){
            for(auto &it : inp){
                if(lookup.count(it)) continue;
                lookup[it] = graph_base<T, Edge>::push_back(it);
            }
        }

        //copy constructor
        graph(const graph &val): graph_base<T, Edge>(val), lookup{val.lookup}{}

        //copy assignment
        graph& operator= (const graph &val){
            if (this != &val)  {            
                *this = val;
                lookup = val.lookup;
            }  
            return *this;  
        }  

        int push_back(T &t){
            if(lookup.count(t) == 0) return lookup[t] = graph_base<T, Edge>::push_back(t);
            return lookup[t];
        }

        vertex<T, Edge> operator[](int i){
            auto &lookup = this->lookup;
            auto upd = [i, &lookup](const T& oldval, const T& newval){
                if(!(oldval == newval)){
                    if(lookup.count(newval)) throw duplicate_vertex_error();
                    lookup.erase(oldval);
                    lookup[newval] = i;
                }
            };
            return graph_base<T, Edge>::get_vertex(i, upd);
        }

        optional<int> get_index(const T& val){
            auto it = lookup.find(val);
            if(it == lookup.end()) return nullopt;
            return it->second;
        }
    };

    template <class T, class Edge>
    class unordered_graph : public unordered_graph_base<T, Edge> {
    public:
        unordered_graph():unordered_graph_base<T, Edge>() { }
        unordered_graph(int n):unordered_graph_base<T, Edge>(n) { }
        unordered_graph(const initializer_list<T> &inp):unordered_graph_base<T, Edge>(inp) { }
        unordered_graph(const unordered_graph &val):unordered_graph_base<T, Edge>(val) { }
        unordered_graph(const unordered_graph &&val):unordered_graph_base<T, Edge>(val) { *val = nullptr; }
        vertex<T, Edge> operator[](int i){
            auto upd = [](const T& oldval, const T& newval){ };
            return unordered_graph_base<T, Edge>::get_vertex(i, upd);
        }
    };
}

#endif