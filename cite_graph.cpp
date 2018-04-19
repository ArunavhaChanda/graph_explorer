// The main file to run to test Graph Explorer

#include "graph_matrix.h"
#include "graph_list.h"
#include "our_algos.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "boost/graph/topological_sort.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/config.hpp"

using namespace graphlist;

typedef std::pair<std::size_t,std::size_t> Pair;

int main(){
    // another type of graph
    graph<string> cite_graph; // an empty graph.

    ifstream cites;
    cites.open("./Cit-HepTh.txt");
    string line;
    int skip = 0;
    int vert_tot = 0;
    set<int> nodes_added;

    if (cites.is_open()){
        while(getline(cites,line)) {
            if (skip < 4) {
                skip++;
                continue;                
            }

            istringstream linestream(line);
            int node_old, node_new;
            cout <<line<<endl;

            linestream >> node_new >> node_old;
            cout << "node_new: " << node_new << " and node_old: " << node_old <<endl;
            const string new_label_const = to_string(node_new);
            const string old_label_const = to_string(node_old);

            string new_label = new_label_const;
            string old_label = old_label_const;

            auto new_found = nodes_added.find(node_new);
            auto old_found = nodes_added.find(node_old);
            if (new_found == nodes_added.end()){
                cite_graph.push_back(new_label);
                nodes_added.insert(node_new);
                ++vert_tot;
            }
            if (old_found == nodes_added.end()){
                cite_graph.push_back(old_label);
                nodes_added.insert(node_old);
                ++vert_tot;
            }
            auto index_old = cite_graph.get_index(old_label);
            auto index_new = cite_graph.get_index(new_label);
            cout << "old index: " << *index_old << " and new index: " << *index_new <<endl;
            cite_graph.add_edge(*index_old, *index_new);
        }
    }

    if(auto topological_order = get_topological_order(cite_graph)){
        cout << "topological order exists" << '\n';
        for(auto &s : *topological_order) cout << s << '\n';
        cout << '\n';
    }else{
        cout << "topological order does not exist" << '\n';
    }

    cites.close();



    using namespace boost;

    typedef adjacency_list<vecS, vecS, directedS, 
    property<vertex_color_t, default_color_type> > Graph;

    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

    Pair edges[360000];

    ifstream cites_boost;
    cites_boost.open("./Cit-HepTh.txt");
    skip = 0;
    int edge_no = 0;

    if (cites_boost.is_open()){
        while(getline(cites_boost,line)) {
            if (skip < 4) {
                skip++;
                continue;                
            }

            istringstream linestream(line);
            int node_old, node_new;
            cout <<line<<endl;

            linestream >> node_new >> node_old;
            cout << "node_new: " << node_new << " and node_old: " << node_old <<endl;
            edges[edge_no] = Pair(node_old,node_new);
            ++edge_no;
        }
    }
    
    cites_boost.close();

    Graph G(edges, edges + edge_no, edge_no);
    
    boost::property_map<Graph, vertex_index_t>::type id = get(vertex_index, G);
    typedef std::vector< Vertex > container;
    container c;
    topological_sort(G, std::back_inserter(c));
    std::cout << "A topological ordering: ";
    for (container::reverse_iterator ii = c.rbegin(); 
       ii != c.rend(); ++ii)
    std::cout << id[*ii] << " ";
    std::cout << std::endl;

    return 0;
}
