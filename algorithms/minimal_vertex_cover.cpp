#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>

using vertex_t = uint32_t;

using Graph = std::vector<std::unordered_map<vertex_t, bool>>;

void add_edge(Graph& adj_list, vertex_t from, vertex_t to) {
    // create edge in the Graph
    adj_list[from].emplace(to, false);
}


void dfs(const Graph& graph, std::vector<bool>& visited, vertex_t u) {
    visited[u] = true;
    for (auto [v, _] : graph[u]) {
        if(!visited[v]){
            dfs(graph, visited, v);
        }
    }
}

int main() {
    size_t m, n;
    std::cin >> m >> n;
    size_t k;
    vertex_t v;
    Graph graph(m + n);
    for (size_t u = 0; u < m; ++u) {
        std::cin >> k;
        for(size_t j = 0; j < k; ++j) {
            std::cin >> v;
            add_edge(graph, u, v - 1 + m);
        }
    }

    vertex_t l;
    std::vector<vertex_t> free;
    Graph reversed_graph(m + n);

    for(size_t i = 0; i < m; ++i) {
        std::cin >> l;
        if (l != 0) {
            add_edge(reversed_graph, l - 1 + m, i);
//            assert(i < Graph.get_size());
//            assert(left - 1 < Graph[i].get_size());
            graph[i][l - 1] = true;
        } else {
            free.push_back(i);
        }
    }

    for(vertex_t i = 0; i < m + n; ++i) {
        for (auto [vertex, deleted] : graph[i]) {
            if (!graph[i][vertex]) {
                add_edge(reversed_graph, i, vertex);
            }
        }
    }

    std::vector<bool> visited(m + n, false);
    for (unsigned int i : free) {
        dfs(reversed_graph, visited, i);
    }

    std::vector<vertex_t> l_minus;
    for (vertex_t i = 0; i < m; i++) {
        if (!visited[i]) {
            l_minus.push_back(i + 1);
        }
    }

    std::vector<vertex_t> r_plus;
    for (vertex_t i = m; i < m + n; i++) {
        if (visited[i]) {
            r_plus.push_back(i + 1 - m);
        }
    }

    std::cout << l_minus.size() + r_plus.size() << std::endl;
    std::cout << l_minus.size() << " ";

    for(auto a : l_minus) {
        std::cout << a << " ";
    }

    std::cout << std::endl << r_plus.size() << " ";

    for(auto a : r_plus) {
        std::cout << a << " ";
    }
}