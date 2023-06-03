#include <vector>
#include <iostream>
#include <cmath>
#include <map>
#include <unordered_map>

size_t logn = 1;

using vertex_t = int64_t;
using weight_t = int64_t;
std::vector<weight_t> enter_time;
std::vector<weight_t> exit_time;
std::vector<weight_t> dist;
std::vector<std::vector<weight_t>> binary_ups;

using Row = std::unordered_map<vertex_t, weight_t>;
using Graph =  std::vector<Row>;

weight_t timer = 1;

void precalculate_lca(const Graph& graph, vertex_t v, vertex_t p = 0, weight_t len = 0) {
    enter_time[v] = timer++;
    binary_ups[v][0] = p;
    dist[v] = len;

    for(size_t i = 1; i <= logn; i++) {
        binary_ups[v][i] = binary_ups[binary_ups[v][i - 1]][i - 1];
    }
    for(auto [to, w] : graph[v]) {
        if (to != p) {
            precalculate_lca(graph, to, v, len + w);
        }
    }
    exit_time[v] = timer++;
}

vertex_t get_parent(vertex_t u, vertex_t v) {
    return (enter_time[u] <= enter_time[v]) && (exit_time[u] >= exit_time[v]);
}

vertex_t get_lca(vertex_t a, vertex_t b) {
    if (get_parent(a, b)) {
        return a;
    }
    if (get_parent(b, a)) {
        return b;
    }
    for (int i = logn - 1; i >= 0; i--) {
        if (!get_parent(binary_ups[a][i], b)) {
            a = binary_ups[a][i];
        }
    }
    return binary_ups[a][0];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t n;
    std::cin >> n;
//    logn = static_cast<int>(std::log(n));
    while ((1 << logn) <= n)  {
        logn++;
    }
    enter_time.resize(n);
    dist.resize(n);
    exit_time.resize(n);

    binary_ups.resize(n);
    for (auto&& u : binary_ups) {
        u.resize(logn + 1);
    }

    vertex_t u, v;
    weight_t w;
    Graph graph(n);
    for (size_t i = 0; i < n - 1; i++) {
        std::cin >> u >> v >> w;
        graph[u][v] = w;
        graph[v][u] = w;
    }

    vertex_t root = 0;
    precalculate_lca(graph, root);

    size_t m;
    std::cin >> m;
    for (size_t i = 0; i < m; i++) {
        std::cin >> u >> v;
        auto lca = get_lca(u, v);
        std::cout <<  dist[u] - dist[lca] + dist[v] - dist[lca] << "\n";

    }
}