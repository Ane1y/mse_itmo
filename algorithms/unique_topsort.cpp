#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using adjacency_list = std::vector<std::vector<uint32_t>>;

struct times {
    uint32_t enter_time;
    uint32_t exit_time;
};

void explore(uint32_t v, const adjacency_list& adj_list, std::vector<bool>& visited, std::vector<times>& times, uint32_t& cur_time) {
    visited[v] = true;
    times[v] = {cur_time++, times[v].exit_time};
    for (auto u: adj_list[v]) {
        if (!visited[u]) {
            explore(u, adj_list, visited, times, cur_time);
        }
    }
    times[v] = {times[v].enter_time, cur_time++};
}

std::vector<times> dfs(const adjacency_list& adj_list) {
    std::vector<times> times(adj_list.size(), {0, 0});
    std::vector<bool> visited(adj_list.size(), false);
    uint32_t cur_time = 0;
    for (uint32_t v = 0; v < adj_list.size(); ++v) {
        if (!visited[v]) {
            explore(v, adj_list, visited, times, cur_time);
        }
    }
    return times;
}

std::vector<uint32_t> top_sort(const std::vector<times>& times) {
    std::vector<uint32_t> ts_vertices(times.size());
    std::iota(ts_vertices.begin(), ts_vertices.end(), 0);
    std::sort(ts_vertices.begin(), ts_vertices.end(), [&times](uint32_t i, uint32_t j){
        return times[i].exit_time > times[j].exit_time;}
    );
    return ts_vertices;

}
bool is_top_sort_unique(const adjacency_list& adj_list, const std::vector<uint32_t>& vertices) {
    for (uint32_t i = 0; i < adj_list.size() - 1; ++i) {
        bool founded = false;
        auto cur_v = vertices[i];
        auto next_v = vertices[i + 1];

        for(uint32_t adj_v : adj_list[cur_v]) {
            if (adj_v == next_v) {
                founded = true;
                continue;
            }
        }
        if (!founded) {
            return false;
        }
    }
    return true;
}


int main() {
    uint32_t n, m;
    std::cin >> n >> m;

    adjacency_list adj_list(n);

    uint32_t v, u;
    for (uint32_t i = 0; i < m; ++i) {
        std::cin >> v >> u;
        adj_list[v - 1].push_back(u - 1);
    }

    auto times = dfs(adj_list);
    auto top_sorted_vertices = top_sort(times);

    if (is_top_sort_unique(adj_list, top_sorted_vertices)) {
        std::cout << "YES" << std::endl;
        for (auto i : top_sorted_vertices) {
            std::cout << i + 1 << " ";
        }
    } else {
        std::cout << "NO" << std::endl;
    }

    return 0;
}
