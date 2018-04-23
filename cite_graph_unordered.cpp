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
#include <unordered_map>
#include <chrono>
#include "boost/graph/topological_sort.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/exception/diagnostic_information.hpp"
#include "boost/exception_ptr.hpp"
#include "boost/config.hpp"

using namespace NMSPC;

struct string_wrap {
    std::string s;
};

int main(){
    // another type of graph
    unordered_graph<string_wrap> cite_graph; // an empty graph.

    ifstream cites;
    cites.open("./cit-Patents.txt");
    //cites.open("./Cit-HepTh.txt");
    string line;
    int skip = 0;
    int vert_tot = 0;
    set<int> nodes_added;
    unordered_map<string_wrap,int> indices;

    if (cites.is_open()){
        while(getline(cites,line)) {
            if (skip < 4) {
                skip++;
                continue;                
            }

            istringstream linestream(line);
            int node_old, node_new;
            //cout <<line<<endl;

            linestream >> node_new >> node_old;
            //cout << "node_new: " << node_new << " and node_old: " << node_old <<endl;
            const string new_label_const = to_string(node_new);
            const string old_label_const = to_string(node_old);

            string_wrap new_label;
            string_wrap old_label;

            new_label.s = new_label_const;
            old_label.s = old_label_const;

            auto new_found = nodes_added.find(node_new);
            auto old_found = nodes_added.find(node_old);
            if (new_found == nodes_added.end()){
                int index = cite_graph.push_back(new_label);
                indices[new_label] = index;
                nodes_added.insert(node_new);
                ++vert_tot;
                if(vert_tot%10000 == 0)
                    cout <<vert_tot<<endl;
            }
            if (old_found == nodes_added.end()){
                int index = cite_graph.push_back(old_label);
                indices[old_label] = index;
                nodes_added.insert(node_old);
                ++vert_tot;
                if(vert_tot%10000 == 0)
                    cout <<vert_tot<<endl;
            }


            iterator it_old = indices.find(old_label);
            auto index_old = it_old->second;

            iterator it_new = indices.find(new_label);
            auto index_new = it_new->second;

            //cout << "old index: " << *index_old << " and new index: " << *index_new <<endl;
            cite_graph.add_edge(*index_old, *index_new);
        }
    }

    auto start = std::chrono::system_clock::now();
    auto topological_order = get_topological_order(cite_graph);
    auto end = std::chrono::system_clock::now(); 
    std::chrono::duration<double> elapsed_seconds = end-start;

    if(topological_order){
        cout << "topological order exists" << '\n';
        /*
        for(auto &s : *topological_order) cout << cite_graph[s].val << '\n';
        cout << '\n';
        */
    }else{
        cout << "topological order does not exist" << '\n';
    }

    std::cout << "top-sort time for graph_explorer is: " << elapsed_seconds.count() << "s\n";

    cites.close();



    using namespace boost;

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

    Graph g(vert_tot);

    //Pair edges[1100000];

    ifstream cites_boost;
    cites_boost.open("./cit-Patents.txt");
    //cites_boost.open("./Cit-HepTh.txt");
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
            //cout <<line<<endl;

            linestream >> node_new >> node_old;
            //cout << "node_new: " << node_new << " and node_old: " << node_old <<endl;
            //edges[edge_no] = Pair(node_old,node_new);

            const string new_label_const = to_string(node_new);
            const string old_label_const = to_string(node_old);

            string_wrap new_label;
            string_wrap old_label;

            new_label.s = new_label_const;
            old_label.s = old_label_const;

            iterator it_old = indices.find(old_label);
            auto index_old = it_old->second;

            iterator it_new = indices.find(new_label);
            auto index_new = it_new->second;


            boost::add_edge(*index_old, *index_new, g);
            ++edge_no;
        }
    }
    
    cites_boost.close();

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
        cout << "top-sort time for boost is: " << elapsed_seconds_boost.count() << "s\n";

        /*
        for ( container::reverse_iterator ii=c.rbegin(); ii!=c.rend(); ++ii)
            cout << cite_graph[id[*ii]].val << "\n";
        cout << endl;
        */
    }
    catch (boost::exception const& e) {
        auto end_boost = std::chrono::system_clock::now(); 
        std::chrono::duration<double> elapsed_seconds_boost = end_boost-start_boost;
        cout << "top-sort time for boost is: " << elapsed_seconds_boost.count() << "s\n";
        std::string diag = boost::diagnostic_information(e);
        printf("Boost exception: %s", diag);
    }


    return 0;
}
