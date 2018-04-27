// The main file to run to test Graph Explorer

#include "../graph_matrix.h"
#include "../graph_list.h"
#include "../our_algos.h"
#include "../our_constants.h"
#include "boost/config.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
 
#include "boost/graph/edmonds_karp_max_flow.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/read_dimacs.hpp"
#include "boost/graph/graph_utility.hpp"

using namespace NMSPC;

void graph_explorer_max_flow()
{

    graph<string, int> flow_graph; // an empty graph.

    ifstream flows;
    flows.open("./BVZ-tsukuba0.max");
    string line;
    string source, sink;
    int skip = 0;
    int vert_tot = 0;
    set<int> nodes_added;

    auto start_init = std::chrono::system_clock::now();

    if (flows.is_open()) {
        while(getline(flows,line)) {
            if (line[0] != 'a') {
                if (line[0] == 'n') {
                    istringstream linestream(line);
                    int node_id;
                    char info, descriptor;

                    linestream >> info >> node_id >> descriptor;
                    if (descriptor == 's')
                        source = to_string(node_id);
                    else if (descriptor == 't')
                        sink = to_string(node_id);
                }
                continue;
            }

            istringstream linestream(line);
            char type;
            int node_out, node_in, weight;
            //cout <<line<<endl;

            linestream >> type >> node_out >> node_in >> weight;
            //cout << "node_new: " << node_new << " and node_old: " << node_old <<endl;
            const string out_label_const = to_string(node_out);
            const string in_label_const = to_string(node_in);

            string in_label = in_label_const;
            string out_label = out_label_const;

            auto in_found = nodes_added.find(node_in);
            auto out_found = nodes_added.find(node_out);
            if (in_found == nodes_added.end()){
                flow_graph.push_back(in_label);
                nodes_added.insert(node_in);
                ++vert_tot;
            }
            if (out_found == nodes_added.end()){
                flow_graph.push_back(out_label);
                nodes_added.insert(node_out);
                ++vert_tot;
            }
            auto index_out = flow_graph.get_index(out_label);
            auto index_in = flow_graph.get_index(in_label);
            //cout << "old index: " << *index_old << " and new index: " << *index_new <<endl;
            flow_graph.add_edge(*index_out, *index_in, weight);
        }
    }
    auto source_index = flow_graph.get_index(source);
    auto sink_index = flow_graph.get_index(sink);
    auto end_init = std::chrono::system_clock::now(); 
    std::chrono::duration<double> elapsed_seconds_init = end_init-start_init;
    std::cout << "Graph init time for graph_explorer is: " << elapsed_seconds_init.count() << "s\n";

    auto start = std::chrono::system_clock::now();
    int max_flow_val = max_flow(flow_graph, *source_index, *sink_index);

    auto end = std::chrono::system_clock::now(); 
    std::chrono::duration<double> elapsed_seconds = end-start;


    std::cout << "max-flow time for graph_explorer is: " << elapsed_seconds.count() << "s\n";

    cout << "Max flow here is " << max_flow_val << '\n';

    return;
}


void boost_max_flow()
{
    auto start_init = std::chrono::system_clock::now();

    typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS > Traits;
    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
        boost::property<boost::vertex_name_t, std::string >,
        boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
        boost::property<boost::edge_reverse_t, Traits::edge_descriptor>>>> Graph;
 


    Graph g;
 
    boost::property_map<Graph, boost::edge_capacity_t>::type
        capacity = get(boost::edge_capacity, g);
    boost::property_map<Graph, boost::edge_reverse_t>::type rev = get(boost::edge_reverse, g);
    boost::property_map<Graph, boost::edge_residual_capacity_t>::type
        residual_capacity = get(boost::edge_residual_capacity, g);
 
    Traits::vertex_descriptor s, t;
     
    // Use a DIMACS network flow file as stdin:
    std::ifstream is ("BVZ-tsukuba0.max", std::ios::in);
    read_dimacs_max_flow(g, capacity, rev, s, t, is);
 
    auto end_init = std::chrono::system_clock::now(); 
    std::chrono::duration<double> elapsed_seconds_init = end_init-start_init;
    std::cout << "Graph init time for boost is: " << elapsed_seconds_init.count() << "s\n";

    auto start = std::chrono::system_clock::now();

    #if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
      std::vector<default_color_type> color(num_vertices(g));
      std::vector<Traits::edge_descriptor> pred(num_vertices(g));
      long flow = edmunds_karp_max_flow
        (g, s, t, capacity, residual_capacity, rev, &color[0], &pred[0]);
    #else
      long flow = edmonds_karp_max_flow(g, s, t);
    #endif
 
    auto end = std::chrono::system_clock::now(); 
    std::chrono::duration<double> elapsed_seconds = end-start;


    std::cout << "max-flow time for boost is: " << elapsed_seconds.count() << "s\n";

    std::cout << "c  The total flow:" << std::endl;
    std::cout << "s " << flow << std::endl << std::endl;
 
    return;
}

int main()
{
    cout << "\n\n ----- MAX FLOW -----";
    graph_explorer_max_flow();
    boost_max_flow();
    return 0;
}
