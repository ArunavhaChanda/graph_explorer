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
#include <map>
#include <chrono>
#include "boost/graph/topological_sort.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/exception/diagnostic_information.hpp"
#include "boost/exception_ptr.hpp"
#include "boost/config.hpp"

using namespace NMSPC;

int main(){
    // another type of graph
    graph<string> class_graph; // an empty graph.

    ifstream classes;
    classes.open("./columbia_ce_reqs.txt");
    //classes.open("./Cit-HepTh.txt");
    string line;
    int vert_tot = 0;
    set<string> nodes_added;

    // 790 726
    if (classes.is_open()) {
        while(getline(classes,line)) {
            istringstream linestream(line);
            string new_label, old_label;
            //cout <<line<<endl;

            linestream >> old_label >> new_label;
            //cout << "node_new: " << node_new << " and node_old: " << node_old <<endl;

            auto new_found = nodes_added.find(new_label);
            auto old_found = nodes_added.find(old_label);
            if (new_found == nodes_added.end()){
                class_graph.push_back(new_label);
                nodes_added.insert(new_label);
                ++vert_tot;
            }
            if (old_found == nodes_added.end()){
                class_graph.push_back(old_label);
                nodes_added.insert(old_label);
                ++vert_tot;
            }
            auto index_old = class_graph.get_index(old_label);
            auto index_new = class_graph.get_index(new_label);
            //cout << "old index: " << *index_old << " and new index: " << *index_new <<endl;
            class_graph.add_edge(*index_old, *index_new);
        }
    }


    auto start = std::chrono::system_clock::now();
    auto topological_order = get_topological_order(class_graph);
    auto end = std::chrono::system_clock::now(); 
    std::chrono::duration<double> elapsed_seconds = end-start;

    if(topological_order){
        cout << "topological order exists" << '\n';
        for(auto &s : *topological_order) cout << class_graph[s].val << '\n';
        cout << '\n';
    }else{
        cout << "topological order does not exist" << '\n';
    }

    std::cout << "top-sort time for graph_explorer is: " << elapsed_seconds.count() * 1000 << "ms\n";

    classes.close();



    using namespace boost;

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

    Graph g(vert_tot);

    //Pair edges[1100000];

    ifstream classes_boost;
    classes_boost.open("./columbia_ce_reqs.txt");
    //classes_boost.open("./Cit-HepTh.txt");
    int edge_no = 0;

    if (classes_boost.is_open()){
        while(getline(classes_boost,line)) {
            istringstream linestream(line);
            string new_label, old_label;
            //cout <<line<<endl;

            linestream >> old_label >> new_label;
            //cout << "node_new: " << node_new << " and node_old: " << node_old <<endl;

            auto new_found = nodes_added.find(new_label);
            auto old_found = nodes_added.find(old_label);
            if (new_found == nodes_added.end()){
                class_graph.push_back(new_label);
                nodes_added.insert(new_label);
            }
            if (old_found == nodes_added.end()){
                class_graph.push_back(old_label);
                nodes_added.insert(old_label);
            }
            auto index_old = class_graph.get_index(old_label);
            auto index_new = class_graph.get_index(new_label);

            boost::add_edge(*index_old, *index_new, g);
            ++edge_no;
        }
    }
    
    classes_boost.close();

    //Graph G(edges, edges + edge_no, edge_no);
    
    boost::property_map<Graph, vertex_index_t>::type id = get(vertex_index, g);
    typedef std::vector< Vertex > container;
    container c;
    auto start_boost = std::chrono::system_clock::now();
    try{
        topological_sort(g, std::back_inserter(c));
        auto end_boost = std::chrono::system_clock::now(); 
        std::chrono::duration<double> elapsed_seconds_boost = end_boost-start_boost;

        cout << "A topological ordering exists\n";
        cout << "top-sort time for boost is: " << elapsed_seconds_boost.count() * 1000 << "ms\n";
        for ( container::reverse_iterator ii=c.rbegin(); ii!=c.rend(); ++ii)
            cout << class_graph[id[*ii]].val << "\n";
        cout << endl;
    }
    catch (boost::exception const& e) {
        auto end_boost = std::chrono::system_clock::now(); 
        std::chrono::duration<double> elapsed_seconds_boost = end_boost-start_boost;
        cout << "top-sort time for boost is: " << elapsed_seconds_boost.count() * 1000 << "ms\n";
        std::string diag = boost::diagnostic_information(e);
        printf("Boost exception: %s", diag);
    }


    return 0;
}
