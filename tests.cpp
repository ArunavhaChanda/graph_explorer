// The main file to run to test Graph Explorer

#include "graph_matrix.h"
#include "graph_list.h"
#include "our_algos.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

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
