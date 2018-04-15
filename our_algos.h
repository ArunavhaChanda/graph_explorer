// Definitions of the algorithms we will use to test the code

#include <vector>
#include <set>
#include <optional>
#include <stack>
#include <queue>

using namespace std;

template <class T>
int max_flow(T &g, int src, int target){
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
optional<vector<int>> get_topological_order(T &g){
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
