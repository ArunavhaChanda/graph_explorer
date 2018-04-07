#include <vector>
#include <optional>
#include <algorithm>
#include <map>
#include <cassert>
#include <iostream>
#include <set>
#include <stack>
#include <climits>
#include <queue>
#include <set>
#include <functional>

using namespace std;
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
class graph{
    // Think about doing map<pair<int, int>, int> adj;
    map<int, map<int, int>> adj;
    vector<T> node;
    map<T, int> lookup;
public:
    graph(){}
    graph(int n){
        node.resize(n);
    }
    graph(initializer_list<T> inp){
        for(auto it=inp.begin();it!=inp.end();it++){
            if(lookup.find(*it) == lookup.end()){
                lookup[*it] = node.size();
                node.push_back(*it);
            }
        }
    }

    int push_back(T &t){
        int res = node.size();
        node.push_back(t);
        return res;
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

    // T& operator[](int i){
        // assert(i < node.size());
    //     return node[i];
    // }

    // TODO: Handle cases where newval already exists in the lookup dictionary..
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

template <class T>
int max_flow(graph<T> &g, int src, int target){
    assert(src < g.nodes().size() && target < g.nodes().size());
    if(src == target) return INT_MAX;

    // Edmond karp
    auto adj = g.edges();
    int max_flow = 0;
    bool found = true;
    while(found){
        found = false;
        set<int> visited; visited.insert(src);
        queue<int> que; que.push(src);
        map<int, int> parent;
        while(!que.empty()){
            auto cur = que.front(); que.pop();
            for(auto &p : adj[cur]){
                int i = p.first;
                if(visited.count(i) || p.second == 0) continue;
                parent[i] = cur;
                if(i == target){
                    found = true;
                    break;
                }
                que.push(i); visited.insert(i);
            }
            if(found) break;
        }

        if(found){
            auto iter = target;
            int flow = INT_MAX;
            while(iter != src){
                flow = min(flow, adj[parent[iter]][iter]);
                iter = parent[iter];
            }
            max_flow += flow;
            iter = target;
            while(iter != src){
                adj[parent[iter]][iter] -= flow;
                adj[iter][parent[iter]] += flow;
                iter = parent[iter];
            }
        }
    }

    return max_flow;
}

template <class T>
optional<vector<int>> get_topological_order(graph<T> &g){
    vector<int> order;
    int n = g.size();
    auto adj = g.edges();
    set<int> visited, visiting;
    stack<pair<int, pair<map<int, int>::iterator, map<int, int>::iterator>>> stk;
    for(int i=0;i<n;i++){
        if(visited.count(i)) continue;

        stk.push({i, {adj[i].begin(), adj[i].end()}});
        visiting.insert(i);
        while(!stk.empty()){
            auto cur = stk.top(); stk.pop();
            if(cur.second.first == cur.second.second){
                visiting.erase(cur.first);
                visited.insert(cur.first);
                order.push_back(cur.first);
            }
            else{
                pair<int, int> it = *(cur.second.first);
                ++cur.second.first;
                stk.push({cur.first,{cur.second.first, cur.second.second}});

                if(visiting.count(it.first)) return nullopt;
                if(visited.count(it.first)) continue;
                visiting.insert(it.first);
                stk.push({it.first, {adj[it.first].begin(), adj[it.first].end()}});    
            }
        }
    }

    reverse(order.begin(), order.end());
    return order;
}

int main(){
    graph<string> g3{"New York"s, "Chicago"s, "Seattle"s, "Boston"s};

    auto v = g3[0];
    v = "new Yoork";
    cout << g3[0].val << endl;
    cout << v.val << endl;

    
    vector<vector<int>> edges2{{0,1,10},{2,1,12},{3,0,11},{0,3,43}};
    for(auto &i : g3.nodes()) cout << i << ','; cout << '\n';
    for(auto &edge : edges2) g3.add_edge(edge[0], edge[1], edge[2]);
    for(auto &i : g3.edges()){
           for(auto &p : i.second){
               cout << g3[i.first].val << "->" << g3[p.first].val << ':' << p.second << '\n';
           }
    }

    for(auto &p : g3[0]) cout << g3[p.first].val << ' ' << p.second << endl;

    int n = 10;
    graph<int> g(n); 
    for(int i=0;i<n;i++) g[i] = i; // assigning values to vertices.

    vector<vector<int>> edges{{0,1,2},{1,2,3},{0,2,10},{2,3,1}};

    // Signature: g.add_edge(src_vertex_index, target_vertex_index, weight);
    for(auto &edge : edges) g.add_edge(edge[0], edge[1], edge[2]);
    cout << "Max flow between vertex 0 and 2 is " << max_flow(g, 0, 2) << '\n';

    // another type of graph
    graph<string> g2; // an empty graph.


// adding nodes.
    vector<string> node_values{"newyork"s, "seattle"s, "boston"s, "miami"s};
    for(string &s : node_values) g2.push_back(s);

    for(auto &edge : edges) g2.add_edge(edge[0], edge[1], edge[2]);
    cout << g2[1].val << '\n'; // prints "seattle" (without the quotes)

    // Get topological order, appends the order to passed list (using push_back).
    
    if(auto topological_order = get_topological_order(g2)){
        cout << "topological order exists" << '\n';
        for(auto &s : *topological_order) cout << s << ' ';
        cout << '\n';
    }else{
        cout << "topological order does not exist" << '\n';
    }

    g2.add_edge(3,0,1);

    if(auto topological_order = get_topological_order(g2)){
        cout << "topological order exists" << '\n';
        for(auto &s : *topological_order) cout << s << ' ';
        cout << '\n';
    }else{
        cout << "topological order does not exist" << '\n';
    }

    return 0;
}
