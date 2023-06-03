#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <set>

struct Edge {
    int32_t u;
    int32_t v;
};
using adjacency_list = std::vector<std::multiset<int32_t>>;

std::vector<Edge> bridges;


void detect_bridges(int32_t u,
                    int32_t parent,
                    const adjacency_list& adj,
//                    std::vector<bool>& visited,
                    std::vector<size_t>& low,
                    std::vector<size_t>& discovered,
                    size_t& cur_time) {
    cur_time += 1;
    low[u] = cur_time;
    discovered[u] = cur_time;
    for (auto v : adj[u]) {
        if (v == parent) {
            continue;
        }
        if (!discovered[v]) {
            detect_bridges(v, u, adj, low, discovered, cur_time);
            if (discovered[u] < low[v]) {
                bridges.push_back({u, v});
            }
            low[u] = std::min(low[u], low[v]);
        } else {
            low[u] = std::min(low[u], discovered[v]);
        }
    }
}

void explore(int32_t v, const adjacency_list& adj_list, std::vector<bool>& visited, std::vector<int32_t>& component) {
    visited[v] = true;
    component.push_back(v);
    for (auto u: adj_list[v]) {
        if (!visited[u]) {
            explore(u, adj_list, visited, component);
        }
    }
}

std::vector<std::vector<int32_t>> count_components(const adjacency_list& adj) {
    std::vector<std::vector<int32_t>> components;
    std::vector<bool> visited(adj.size(), false);
    for (int32_t v = 0; v < adj.size(); ++v) {
        if (!visited[v]) {
            std::vector<int32_t> cur_component;
            explore(v, adj, visited, cur_component);
            std::sort(cur_component.begin(), cur_component.end());
            components.push_back(cur_component);
        }
    }
    return components;
}


int main() {
    size_t n_vertices, n_edges;
    std::cin >> n_vertices >> n_edges;

    adjacency_list adj(n_vertices);
    int32_t v, u;

    for (size_t i = 0; i < n_edges; ++i) {
        std::cin >> v >> u;
        if (u == v) {
            continue;
        }
        --v;
        --u;
        adj[v].insert(u);
        adj[u].insert(v);
    }
//    std::vector<bool> visited(n_vertices, false);
    std::vector<size_t> low(n_vertices, 0);
    std::vector<size_t> discovered(n_vertices, 0);
    size_t cur_time = 0;
    for (int i = 0; i < n_vertices; ++i) {
        if (!discovered[i]) {
            detect_bridges(i, 0, adj, low, discovered, cur_time);
        }
    }

    for (const auto& [e1, e2] : bridges) {
        adj[e1].erase(adj[e1].find(e2));
        adj[e2].erase(adj[e2].find(e1));
    }

    auto components = count_components(adj);

    std::cout << components.size() << std::endl;
|
    for (const auto& component : components) {
        for (auto c : component) {
            std::cout << c + 1 << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
