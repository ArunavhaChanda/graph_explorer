#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "graph_matrix.h"
#include "graph_list.h"
#include "our_algos.h"
#include "our_constants.h"

using namespace graphmatrix;

TEST_CASE("GM:graph testing on not comparable objects", "[GM:unordered_graph]"){
    struct uncomp {
        int x;
        string s;
    };
    uncomp a{2,"hi"s};
    uncomp b{3,"hi1"s};

    unordered_graph<uncomp, int> g1;
    unordered_graph<uncomp, int> g2(5);
    unordered_graph<uncomp, int> g3{a,b};
    unordered_graph<uncomp, int> g4(g3);
    unordered_graph<uncomp, int> g5 = g4;
    
    SECTION("testing sizes"){
        REQUIRE(g1.size() == 0);
        REQUIRE(g2.size() == 5);
        REQUIRE(g3.size() == 2);
        REQUIRE(g4.size() == 2);
        REQUIRE(g5.size() == 2);
    }

    SECTION("testing push_back"){
        REQUIRE(0 == g1.push_back(a));
    }

    SECTION("testing operator[] and vertex .val"){
        REQUIRE(g3[0].val.x == a.x);
    }

    SECTION("testing add_edge and erase_edge"){
        g2.add_edge(0,1,1);
        g2.erase_edge(0,1);
    }

    SECTION("testing nodes and that copy assignment worked"){
        auto tmp = g5.nodes();
        REQUIRE(tmp[0].x == a.x);
        auto tmp1 = g4.nodes();
        REQUIRE(tmp1[0].x == a.x);
    }
}

TEST_CASE("GM:graph testing on == comparable objects", "[GM:equality_graph]"){
    struct eqcomp {
        int x;
        string s;
        bool operator==(const eqcomp& other) const{
            return x == other.x;
        }
    };
    eqcomp a{2,"hi"s};
    eqcomp b{3,"hi1"s};
    eqcomp c{4,"hi2"s};

    graph<eqcomp, int> g1;
    graph<eqcomp, int> g2(5);
    graph<eqcomp, int> g3{a,b};
    graph<eqcomp, int> g4(g3);
    graph<eqcomp, int> g5 = g4;
    
    SECTION("testing sizes"){
        REQUIRE(g1.size() == 0);
        REQUIRE(g2.size() == 5);
        REQUIRE(g3.size() == 2);
        REQUIRE(g4.size() == 2);
        REQUIRE(g5.size() == 2);
    }

    SECTION("testing get_index"){
        REQUIRE(0 == g5.get_index(a));
        REQUIRE(1 == g4.get_index(b));
    }

    SECTION("testing count"){
        REQUIRE(1 == g3.count(a));
        REQUIRE(0 == g3.count(c));
    }

    SECTION("testing push_back"){
        REQUIRE(0 == g1.push_back(a));
    }

    SECTION("testing operator[] and vertex .val"){
        REQUIRE(g3[0].val == a);
    }

    SECTION("testing add_edge and erase_edge"){
        g2.add_edge(0,1, 1);
        g2.erase_edge(0,1);
    }

    SECTION("testing nodes and that copy assignment worked"){
        auto tmp = g5.nodes();
        REQUIRE(tmp[0] == a);
        auto tmp1 = g4.nodes();
        REQUIRE(tmp1[0] == a);
    }
}

TEST_CASE("GM:graph testing on < comparable objects", "[GM:lessthan_graph]"){
    string a = "hi"s;
    string b = "hi1"s;
    string c = "hi2"s;

    graph<string, int> g1;
    graph<string, int> g2(5);
    graph<string, int> g3{a,b};
    graph<string, int> g4(g3);
    graph<string, int> g5 = g4;
    
    SECTION("testing sizes"){
        REQUIRE(g1.size() == 0);
        REQUIRE(g2.size() == 5);
        REQUIRE(g3.size() == 2);
        REQUIRE(g4.size() == 2);
        REQUIRE(g5.size() == 2);
    }

    SECTION("testing get_index"){
        REQUIRE(0 == g5.get_index(a));
        REQUIRE(1 == g4.get_index(b));
    }

    SECTION("testing count"){
        REQUIRE(1 == g3.count(a));
        REQUIRE(0 == g3.count(c));
    }

    SECTION("testing push_back"){
        REQUIRE(0 == g1.push_back(a));
    }

    SECTION("testing operator[] and vertex .val"){
        REQUIRE(g3[0].val == a);
    }

    SECTION("testing add_edge and erase_edge"){
        g2.add_edge(0,1,1);
        g2.erase_edge(0,1);
    }

    SECTION("testing nodes and that copy assignment worked"){
        auto tmp = g5.nodes();
        REQUIRE(tmp[0] == a);
        auto tmp1 = g4.nodes();
        REQUIRE(tmp1[0] == a);
    }
}


using namespace graphlist;

TEST_CASE("GL:graph testing on not comparable objects", "[GL:unordered_graph]"){

    struct uncomp {
        int x;
        string s;
    };
    uncomp a{2,"hi"s};
    uncomp b{3,"hi1"s};

    graphlist::unordered_graph<uncomp, int> g1;
    graphlist::unordered_graph<uncomp, int> g2(5);
    graphlist::unordered_graph<uncomp, int> g3{a,b};
    graphlist::unordered_graph<uncomp, int> g4(g3);
    graphlist::unordered_graph<uncomp, int> g5 = g4;
    
    SECTION("testing sizes"){
        REQUIRE(g1.size() == 0);
        REQUIRE(g2.size() == 5);
        REQUIRE(g3.size() == 2);
        REQUIRE(g4.size() == 2);
        REQUIRE(g5.size() == 2);
    }

    SECTION("testing push_back"){
        REQUIRE(0 == g1.push_back(a));
    }

    SECTION("testing operator[] and vertex .val"){
        REQUIRE(g3[0].val.x == a.x);
    }

    SECTION("testing add_edge and erase_edge"){
        g2.add_edge(0,1,1);
        g2.erase_edge(0,1);
    }

    SECTION("testing nodes and that copy assignment worked"){
        auto tmp = g5.nodes();
        REQUIRE(tmp[0].x == a.x);
        auto tmp1 = g4.nodes();
        REQUIRE(tmp1[0].x == a.x);
    }
}

TEST_CASE("GL:graph testing on == comparable objects", "[GL:equality_graph]"){
    struct eqcomp {
        int x;
        string s;
        bool operator==(const eqcomp& other) const{
            return x == other.x;
        }
    };
    eqcomp a{2,"hi"s};
    eqcomp b{3,"hi1"s};
    eqcomp c{4,"hi2"s};

    graphlist::graph<eqcomp, int> g1;
    graphlist::graph<eqcomp, int> g2(5);
    graphlist::graph<eqcomp, int> g3{a,b};
    graphlist::graph<eqcomp, int> g4(g3);
    graphlist::graph<eqcomp, int> g5 = g4;
    
    SECTION("testing sizes"){
        REQUIRE(g1.size() == 0);
        REQUIRE(g2.size() == 5);
        REQUIRE(g3.size() == 2);
        REQUIRE(g4.size() == 2);
        REQUIRE(g5.size() == 2);
    }

    SECTION("testing get_index"){
        REQUIRE(0 == g5.get_index(a));
        REQUIRE(1 == g4.get_index(b));
    }

    SECTION("testing count"){
        REQUIRE(1 == g3.count(a));
        REQUIRE(0 == g3.count(c));
    }

    SECTION("testing push_back"){
        REQUIRE(0 == g1.push_back(a));
    }

    SECTION("testing operator[] and vertex .val"){
        REQUIRE(g3[0].val == a);
    }

    SECTION("testing add_edge and erase_edge"){
        g2.add_edge(0,1,1);
        g2.erase_edge(0,1);
    }

    SECTION("testing nodes and that copy assignment worked"){
        auto tmp = g5.nodes();
        REQUIRE(tmp[0] == a);
        auto tmp1 = g4.nodes();
        REQUIRE(tmp1[0] == a);
    }
}

TEST_CASE("GL:graph testing on < comparable objects", "[GL:lessthan_graph]"){
    string a = "hi"s;
    string b = "hi1"s;
    string c = "hi2"s;

    graphlist::graph<string, int> g1;
    graphlist::graph<string, int> g2(5);
    graphlist::graph<string, int> g3{a,b};
    graphlist::graph<string, int> g4(g3);
    graphlist::graph<string, int> g5 = g4;
    
    SECTION("testing sizes"){
        REQUIRE(g1.size() == 0);
        REQUIRE(g2.size() == 5);
        REQUIRE(g3.size() == 2);
        REQUIRE(g4.size() == 2);
        REQUIRE(g5.size() == 2);
    }

    SECTION("testing get_index"){
        REQUIRE(0 == g5.get_index(a));
        REQUIRE(1 == g4.get_index(b));
    }

    SECTION("testing count"){
        REQUIRE(1 == g3.count(a));
        REQUIRE(0 == g3.count(c));
    }

    SECTION("testing push_back"){
        REQUIRE(0 == g1.push_back(a));
    }

    SECTION("testing operator[] and vertex .val"){
        REQUIRE(g3[0].val == a);
    }

    SECTION("testing add_edge and erase_edge"){
        g2.add_edge(0,1,1);
        g2.erase_edge(0,1);
    }

    SECTION("testing nodes and that copy assignment worked"){
        auto tmp = g5.nodes();
        REQUIRE(tmp[0] == a);
        auto tmp1 = g4.nodes();
        REQUIRE(tmp1[0] == a);
    }
}