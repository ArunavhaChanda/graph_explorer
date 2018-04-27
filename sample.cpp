#include "graph_matrix.h"
#include <iostream>
#include <cassert>
using namespace graphmatrix;

int main(){
    unordered_graph<string, double> g;
    g.push_back("New York");
    g.push_back("Seattle");
    g.push_back("Chicago");

    cout << g.size() << '\n';

    g[1][0] = 12.33;
    g[1][2] = 54.22;
    vertex<string, double> v = g[1];
    for(auto &p : v){
        cout << g[1].val << "->" << g[p.first].val << ": " << p.second << '\n';
    }

    return 0;
}