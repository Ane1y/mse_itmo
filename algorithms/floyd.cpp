    #include <vector>
    #include <algorithm>
    #include <cstddef>
    #include <iostream>

    int main() {
        size_t n;
        std::cin >> n;

        std::vector<int> adj_array(n * n);

        for (size_t i = 0; i < n * n; ++i) {
            std::cin >> adj_array[i];
        }

        for (size_t k = 0; k < n; ++k)
            for (size_t i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    adj_array[i * n + j] = std::min(adj_array[i * n + j], adj_array[i * n + k] + adj_array[k * n + j]);

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                std::cout << adj_array[i * n + j] << " ";
            }
            std::cout << "\n";
        }
        return 0;
    }

