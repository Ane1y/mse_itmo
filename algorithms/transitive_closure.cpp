#include <iostream>
#include <vector>

size_t n_vertices = 0;

size_t addr(size_t i, size_t j) {
    return i * n_vertices + j;
}

int main() {
    std::string str;
    std::cin >> n_vertices;

    std::vector<uint8_t> adj(n_vertices * n_vertices, 0);

    for (size_t i = 0; i < n_vertices; ++i) {
        std::cin >> str;
        for (int j = 0; j < n_vertices; ++j) {
            size_t v = static_cast<int>(str[j]) - 48;
            adj[addr(i, j)] = v;
        }
    }

    for (size_t k = 0; k < n_vertices; ++k) {
        for (size_t i = 0; i < n_vertices; ++i) {
            for (size_t j = 0; j < n_vertices; ++j) {
                adj[addr(i, j)] |= (adj[addr(i, k)] & adj[addr(k, j)]);
            }
        }
    }

    for (size_t i = 0; i < n_vertices; ++i) {
        for (size_t j = 0; j < n_vertices; ++j) {
            std::cout << +adj[addr(i, j)];
        }
        std::cout << std::endl;
    }
    return 0;
}