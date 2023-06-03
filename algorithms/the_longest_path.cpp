#include <vector>
#include <cstddef>
#include <cmath>
#include <iostream>

size_t n = 0;
size_t m = 0;

std::vector<int32_t> longest_path;
std::vector<int32_t> path;

size_t ind(int32_t i, int32_t j) {
    return i + j * n;
}

void explore(int32_t current, const std::vector<bool>& adj_matrix, std::vector<bool>& visited_path, int32_t visited) {
    if (visited_path[ind(current, visited)]) {
        return;
    }
    path.push_back(current);
    visited_path[ind(current, visited)] = true;
    visited |= 1 << current;
    for (int32_t i = 0; i < n; ++i) {
        if (!(visited & 1 << i) && adj_matrix[ind(current, i)]) {
            explore(i, adj_matrix, visited_path, visited);
        }
    }
    if (path.size() > longest_path.size()) {
        longest_path = path;
    }
    path.pop_back();
}

void dfs(const std::vector<bool>& adj_matrix, std::vector<bool>& visited_path) {
    int32_t visited = 0;
    for (int32_t i = 0; i < n; ++i) {
        explore(i, adj_matrix, visited_path, visited);
    }
}


int main() {

    std::cin >> n >> m;

    std::vector<bool> adj_matrix(n * n, false);

    int16_t u, v;
    for(size_t i = 0; i < m; ++i) {
        std::cin >> u >> v;
        if (u != v) {
            adj_matrix[ind(u - 1, v - 1)] = true;
        }
    }

    std::vector<bool> visited_path(n * static_cast<size_t>(pow(2, n)), false);
    dfs(adj_matrix, visited_path);

    std::cout << longest_path.size() - 1 << std::endl;
    for (auto u : longest_path) {
        std::cout << u + 1 << " ";
    }
    return 0;
};
