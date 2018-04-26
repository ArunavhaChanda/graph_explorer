//Implementation of our graph using a vector to represent a map/matrix

#include <vector>
#include <optional>
#include <algorithm>
#include <map>
#include <cassert>
#include <iostream>
#include <set>
#include <climits>
#include <exception>
#include <functional>

#ifndef CONCEPTS
#define CONCEPTS
#include "our_concepts.h"
#endif

#ifndef GRAPH_MATRIX
#define GRAPH_MATRIX 1

namespace graphmatrix{
    using namespace std;

    struct duplicate_vertex_error : public exception {
       const char * what () const throw () {
          return "the given vertex already exists";
       }
    };

    //definition of vertex class to be used within graph
    template <class T, NumericType Edge>
    class vertex{
        unordered_map<uint32_t, Edge> &adj;
        const function<void(const T&,const T&)> update;
        T& _val;

    public:
        const T& val;
        vertex(T& rval, function<void(const T&,const T&)> u, unordered_map<uint32_t, Edge> &radj):adj{radj}, update{u}, val{rval}, _val{rval}{}
        void operator =(const T& newval){
            update(val, newval);
            _val = newval;
        }

        typename unordered_map<uint32_t,Edge>::iterator begin(){
            return adj.begin();
        }

        typename unordered_map<uint32_t,Edge>::iterator end(){
            return adj.end();
        }

        Edge& operator[](uint32_t i) {
            return adj[i];
        }
    };

    // Graph that just stores nodes (no equal to or less than operator required for T)
    template <class T, NumericType Edge>
    class unordered_graph_base{
        unordered_map<uint32_t, unordered_map<uint32_t, Edge>> adj;
        vector<T> node;
    protected:
        unordered_graph_base() { }
        
        unordered_graph_base(uint32_t n){
            node.resize(n);
        }

        unordered_graph_base(const initializer_list<T> &inp){
            node = vector<T>(inp.begin(), inp.end());
        }

        vertex<T, Edge> get_vertex(uint32_t i, const function<void(const T&, const T&)> upd){
            assert(i < node.size());
            return vertex<T, Edge>(node[i], upd, adj[i]);
        }

    public:
        //copy constructor
        unordered_graph_base(const unordered_graph_base<T, Edge> &val):
            node(val.node),
            adj(val.adj)  
            {}

        unordered_graph_base(unordered_graph_base<T, Edge> &&val):
            node(move(val.node)),
            adj(move(val.adj))
            {}

        unordered_graph_base& operator= (const unordered_graph_base<T, Edge> &val) = default;
        unordered_graph_base& operator= (unordered_graph_base<T, Edge> &&val) = default;

        virtual vertex<T, Edge> operator[](uint32_t i) = 0;

        uint32_t push_back(const T &t){
            uint32_t pos = node.size();
            node.push_back(t);
            return pos;
        }

        void add_edge(uint32_t src, uint32_t target, const Edge &weight){
            assert(target < node.size() && src < node.size());
            adj[src][target] = weight;
        }

        void add_undirected_edge(uint32_t src, uint32_t target, const Edge &weight){
            add_edge(src, target, weight);
            add_edge(target, src, weight);
        }

        void erase_edge(uint32_t src, uint32_t target){
            assert(target < node.size() && src < node.size());
            if(adj.count(src)){
                adj[src].erase(target);
            }
        }

        void erase_undirected_edge(uint32_t src, uint32_t target){
            erase_edge(src, target);
            erase_edge(target, src);
        }

        const vector<T> &nodes() const{
            return node;
        }

        inline size_t size(){
            return node.size();
        }

        const inline size_t size() const{
            return node.size();
        }
        
    };

    // Graph that stores nodes that support == operator and does not allow duplicates.
    template <EqualityComparable T, NumericType Edge>
    class graph_base : public unordered_graph_base<T, Edge> {
    protected:
        graph_base(): unordered_graph_base<T, Edge>() { }
        
        graph_base(uint32_t n) : unordered_graph_base<T, Edge>(n){ }
        graph_base(const graph_base &val):unordered_graph_base<T, Edge>(val){}
        graph_base(graph_base &&val):unordered_graph_base<T, Edge>(move(val)){}

        graph_base& operator= (const graph_base<T, Edge> &val) = default;
        graph_base& operator= (graph_base<T, Edge> &&val) = default;

    public:
        int count(T& val){
            if(auto t = get_index(val)) return 1;
            return 0;
        }

        const uint32_t count(const T& val)const{
            if(auto t = get_index(val)) return 1;
            return 0;
        }

        virtual optional<uint32_t> get_index(const T& val) = 0;
    };

    //general graph implementation for T that does not support < operator
    template <EqualityComparable T, NumericType Edge>
    class graph : public graph_base<T, Edge> {
    public:
        graph():graph_base<T, Edge>() { }

        graph(uint32_t n):graph_base<T, Edge>(n){ }

        graph(const initializer_list<T> &inp):graph_base<T, Edge>(){
            for(auto &it : inp){
                if(auto t = get_index(it)) continue;
                unordered_graph_base<T, Edge>::push_back(it);
            }
        }

        graph(const graph &val):graph_base<T, Edge>(val){}
        graph(graph &&val):graph_base<T, Edge>(val){}

        graph& operator= (const graph<T, Edge> &val) = default;
        graph& operator= (graph<T, Edge> &&val) = default;

        optional<uint32_t> get_index(const T& val){
            auto &node = unordered_graph_base<T, Edge>::nodes();
            ptrdiff_t pos = distance(node.begin(), find(node.begin(), node.end(), val));
            if (pos == node.size()) return nullopt;
            return pos;
        }

        uint32_t push_back(const T &t){
            if(auto it = get_index(t)) return *it;
            return unordered_graph_base<T, Edge>::push_back(t);
        }

        vertex<T, Edge> operator[](uint32_t i){
            auto upd = [&](const T& oldval, const T& newval){
                if(!(oldval == newval)){
                    if(auto t = this->get_index(newval)) throw duplicate_vertex_error();
                }
            };
            return graph_base<T, Edge>::get_vertex(i, upd);
        }
    };

    //graph class specialization for a T that supports < operator
    template <LessThanComparable T, NumericType Edge>
    class graph<T, Edge> : public graph_base<T, Edge> {
        map<T, uint32_t> lookup;
    public:
        graph():graph_base<T, Edge>(){ }

        graph(uint32_t n):graph_base<T, Edge>(n){ }

        graph(const initializer_list<T> &inp):graph_base<T, Edge>(){
            for(auto &it : inp){
                if(lookup.count(it)) continue;
                lookup[it] = graph_base<T, Edge>::push_back(it);
            }
        }

        graph(const graph &val):lookup{val.lookup},graph_base<T, Edge>(val){}
        graph(graph &&val):lookup{move(val.lookup)},graph_base<T, Edge>(move(val)){}

        graph& operator= (const graph &val) = default;
        graph& operator= (graph &&val) = default;

        uint32_t push_back(const T &t){
            if(lookup.count(t) == 0) return lookup[t] = unordered_graph_base<T, Edge>::push_back(t);
            return lookup[t];
        }

        vertex<T, Edge> operator[](uint32_t i){
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

        optional<uint32_t> get_index(const T& val){
            auto it = lookup.find(val);
            if(it == lookup.end()) return nullopt;
            return it->second;
        }
    };

    template <class T, NumericType Edge>
    class unordered_graph : public unordered_graph_base<T, Edge> {
    public:
        unordered_graph():unordered_graph_base<T, Edge>() { }
        unordered_graph(uint32_t n):unordered_graph_base<T, Edge>(n) { }
        unordered_graph(const initializer_list<T> &inp):unordered_graph_base<T, Edge>(inp) { }
        unordered_graph(const unordered_graph &val):unordered_graph_base<T, Edge>(val) { }
        unordered_graph(const unordered_graph &&val):unordered_graph_base<T, Edge>(move(val)) { }
        unordered_graph& operator= (const unordered_graph &val) = default;
        unordered_graph& operator= (unordered_graph &&val) = default;        
        vertex<T, Edge> operator[](uint32_t i){
            auto upd = [](const T& oldval, const T& newval){ };
            return unordered_graph_base<T, Edge>::get_vertex(i, upd);
        }
    };
}

#endif