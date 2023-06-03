#include <iostream>
#include <vector>
#include <limits>

using adjacency_list = std::vector<std::vector<std::pair<size_t, int32_t>>>; //first is vertex, second is cost

size_t n_vertices;

size_t addr(size_t i, size_t j) {
    return i * n_vertices + j;
}
void dfs(uint32_t v, const adjacency_list& adj, std::vector<bool>& visited) {
    visited[v] = true;
    for (const auto& [u, _] : adj[v]) {
        if (!visited[u]) {
            dfs(u, adj, visited);
        }
    }
}

void bellman_ford(const adjacency_list& adj, std::vector<int64_t>& dist, std::vector<size_t>& prev, std::vector<size_t>& loop_v, size_t source) {
    //init
    dist[source] = 0;
    for (size_t i = 0; i < adj.size(); ++i) {
        for (size_t v = 0; v < adj.size(); ++v) {
            for (const auto &[u, w]: adj[v]) {
                if (dist[v] > std::numeric_limits<int64_t>::min() and dist[u] < dist[v] + w) {
                    dist[u] = dist[v] + w;
                    prev[u] = v;
                }
            }
        }
    }

    for (size_t v = 0; v < adj.size(); ++v) {
        for (const auto &[u, w]: adj[v]) {
            auto l = dist[v] + w;
            if (dist[v] > std::numeric_limits<int64_t>::min() and dist[u] < l) {
                loop_v.push_back(u);
            }
        }
    }
}
int main() {

    size_t n_edges;
    std::cin >> n_vertices >> n_edges;

    size_t from, to;
    int32_t score;

    adjacency_list adj(n_vertices);
    for(size_t i = 0; i < n_edges; ++i) {
        std::cin >> from >> to >> score;
        from -= 1;
        to -= 1;
        adj[from].emplace_back(to, score);
    }
    std::vector<int64_t> dist(adj.size(), std::numeric_limits<int64_t>::min());
    std::vector<size_t> prev(adj.size(), -1);
    std::vector<size_t> loop_v;
    bellman_ford(adj, dist, prev, loop_v,  0);
    if (dist[n_vertices - 1] > std::numeric_limits<int64_t>::min()) {
        if (!loop_v.empty()) {
            std::vector<bool> visited(n_vertices, false);
            for (auto v : loop_v) {
                dfs(v, adj, visited);
                if (visited[n_vertices - 1]) {
                    std::cout << ":)";
                    return 0;
                }
            }
        }
        std::cout << dist[n_vertices - 1];
    } else {
        std::cout << ":(";
        return 0;
    }

    return 0;
}

